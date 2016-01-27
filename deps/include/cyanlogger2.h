#pragma once

#include <string>
#include <stdio.h>
#include <vector>
#include <map>
#include <mutex>
#include <thread>

#ifdef CYANLOGGER2_EXPORTS
#define CL2_API __declspec(dllexport)
#else
#define CL2_API __declspec(dllimport)
#endif

namespace cl2
{
	namespace ver
	{
		const unsigned char MAJ = 2;
		const unsigned char MIN = 0;
		// const char *STR = "2.0";
	}

	struct FileHandle
	{
		FILE *file;
		std::string path;
	};
	CL2_API FileHandle *fh_stdout();
	CL2_API FileHandle *fh_stderr();
	CL2_API FileHandle *MakeFH(const std::string& path, const std::string& om = "w");
	CL2_API void FreeFH(FileHandle*& fh);

	// this is almost fully thread safe.
	// almost... lmao
	class Log
	{
	protected:
		bool o;
		std::vector<FileHandle*> dlist;
		std::vector<bool> md;

		std::string fmt_ts;
		std::string fmt_msg;
		std::string fmt_rmsg;
		std::string fmt_ind;
		int ind;

		std::string DoMsgFmt(const std::string& fmt, const std::string& msg, int lvl);
		std::string MakeTS();

		std::string buff;
		unsigned int mbsz;

		std::map<int, std::string> tbl_lvl;
		int deflvl;

		std::mutex op_mtx;
	public:
		// opens a (closed) log, using:
		//    timestamp format=      ""
		//    message format=        "%i%l%m\n"
		//    raw message format=    "%i%m\n"
		//    indent text=           "    "
		//    indent=                0
		//    max buffer size=       100
		//    def lvl table:
		//        ML_NONE=           ""
		//        ML_INFO=           "[INFO] "
		//        ML_WARNING=        "[WARNING] "
		//        ML_ERROR_OK=       "[ERROR--OK] "
		//        ML_ERROR_FATAL=    "[ERROR--FATAL] "
		//    default lvl=           ML_INFO (-2)
		//    enabled=               false
		//   
		CL2_API Log();

		// copies.
		CL2_API Log(const Log&);

		// flushes logs, closes where appropriate.
		CL2_API ~Log();

		// enabling/disabling input. settings can still be changed, but the buffer won't be.
		CL2_API void EnableInput();
		CL2_API void DisableInput();

		// adds a destination to the list.
		// returns true on success, false on failure (invalid destination)
		// if a destination is added by path, the Log instance will remove it.
		// if it is added by FileHandle, the user can specify if the Log instance should remove it or not.
		CL2_API bool AddDest(const std::string& path); // path = "?stdout" or "?stderr" for stdout/stderr, respectively.
		CL2_API bool AddDest(FileHandle *fh, bool autoclose = false);

		// removes a destination from the list.
		// returns true on success, false on failure (destination not in list)
		CL2_API bool RemDest(const std::string& path); // path = "?stdout" or "?stderr" for stdout/stderr, respectively.
		CL2_API bool RemDest(const FileHandle *fh);

		// returns (a copy of) the list of destinations
		CL2_API std::vector<FileHandle*> GetDestList();

		// sets the time-stamp format.
		// format is interpreted as literal text, other than characters that come after '%'.
		// % escape characters are as follows:
		//    %% = %
		//
		//    %s = second
		//    %a = seconds since 1900
		//
		//    %m = minute
		//
		//    %h = hour (12-hr)
		//    %H = hour (24-hr)
		//
		//    %d = day of the month, starting at 1
		//    %w = day of the week (lowercase)
		//    %W = day of the week (UPPERCASE)
		//    %c = day of the week (Capitalized)
		//    %e = abbr day of the week (lowercase)
		//    %E = abbr day of the week (UPPERCASE)
		//    %C = abbr day of the week (Capitalized)
		//    %r = one-letter abbr day of the week (lowercase)
		//    %R = one-letter abbr day of the week (UPPERCASE)
		//
		//    %o = month number, starting at 1
		//    %n = month name (lowercase)
		//    %N = month name (UPPERCASE)
		//    %k = month name (Capitalized)
		//    %j = abbr month name (lowercase)
		//    %J = abbr month name (UPPERCASE)
		//    %K = abbr month name (Capitalized)
		//    
		//    %y = full year (e.g. 2010)
		//    %Y = abbr year (e.g. '10)
		//
		// Where %<other> is read literally (e.g. "%x" -> "%x")
		// Providing an empty timestamp format gets rid of timestamps.
		CL2_API void SetTimestampFormat(const std::string& fmt);
		CL2_API std::string GetTimestampFormat();

		// sets the (normal/raw) message format.
		// format is interpreted as literal text, other than characters that come after '%'.
		// % escape characters are as follows:
		//    %% = %
		//    %l = level
		//    %t = timestamp
		//    %m = message
		//    %i = (correct) indent. this way, message 'blocking' can be turned off.
		// Where %<other> is read literally
		CL2_API void SetMessageFormat(const std::string& fmt);
		CL2_API void SetRawMessageFormat(const std::string& fmt);
		CL2_API std::string GetMessageFormat();
		CL2_API std::string GetRawMessageFormat();

		// sets the indent text.
		CL2_API void SetIndentText(const std::string& ind);
		CL2_API std::string GetIndentText();

		// sets the APPROXIMATE max size of the buffer.
		// A size of zero flushes the buffer on every write.
		CL2_API void SetMaxBuffSize(unsigned int sz);
		CL2_API unsigned int GetMaxBuffSize();

		// these are reserved
		const static int ML_NONE = -1;
		const static int ML_INFO = -2;
		const static int ML_WARNING = -3;
		const static int ML_ERROR_OK = -4;
		const static int ML_ERROR_FATAL = -5;
		const static int ML_DEFAULT = -6;

		// set what ML_DEFAULT corresponds to
		CL2_API void SetDefLvl(int lvl);

		// adds an entry to the message level table
		CL2_API void AddLvlEntry(int lvl, const std::string& msg);
		CL2_API std::map<int, std::string> GetLvlEntryTable();

		// writes a message to the buffer
		CL2_API void PutMsg(const std::string& msg, int lvl = ML_DEFAULT);
		CL2_API void PutRawMsg(const std::string& msg, int lvl = ML_DEFAULT);

		// flushes the buffer
		CL2_API void Flush();

		// indent section control
		CL2_API void EnterSec();
		CL2_API void ExitSec();
		CL2_API void SetSec(unsigned int sec);
		CL2_API unsigned int GetSec();

		// a Write-Only manager.
		// this is useful for giving a module to another class/function,
		// when the other class/function shouldnt be able to change settings.
		class WOM
		{
			bool g;
			cl2::Log *h;
			unsigned __int32 bs;
			unsigned int cs;
			std::mutex op_mtx;
		public:
			CL2_API WOM(); // DO NOT USE THIS (EXCEPT FOR CREATING 'DUMMY LOGS')
			CL2_API ~WOM();

			CL2_API void PutMsg(const std::string& msg, int lvl = ML_DEFAULT);
			CL2_API void PutRawMsg(const std::string& msg, int lvl = ML_DEFAULT);
			CL2_API void EnterSec();
			CL2_API void ExitSec();
			CL2_API void SetSec(unsigned int sec);
			CL2_API unsigned int GetSec();

			// technically a SIBLING WOM, not a child. (same parent Log)
			CL2_API WOM *MakeSubWOM();
			CL2_API WOM *MakeSubWOM(unsigned int bsec);

			friend class cl2::Log;
		};
		CL2_API WOM *MakeWOM(); // USE THIS TO MAKE WOMS
		CL2_API WOM *MakeWOM(unsigned int bsec);
		CL2_API static void FreeWOM(WOM*& wom);
	};
}