		#ifndef _RAR_UI_
		#define _RAR_UI_
		
		// UIERROR_ - error message;
		// UIMSG_   - informational message;
		// UIWAIT_  - message waiting for user confirmation;
		// UIEVENT_ - if simple message is not enough;
		
		enum UIMESSAGE_CODE {
        UIERROR_SYSERRMSG, UIERROR_GENERALERRMSG, UIERROR_INCERRCOUNT,
        UIERROR_CHECKSUM, UIERROR_CHECKSUMENC, UIERROR_CHECKSUMPACKED,
        UIERROR_BADPSW, UIERROR_MEMORY, UIERROR_FILEOPEN, UIERROR_FILECREATE,
        UIERROR_FILECLOSE, UIERROR_FILESEEK, UIERROR_FILEREAD, UIERROR_FILEWRITE,
        UIERROR_FILEDELETE, UIERROR_RECYCLEFAILED, UIERROR_FILERENAME,
        UIERROR_FILEATTR, UIERROR_FILECOPY, UIERROR_FILECOPYHINT,
        UIERROR_DIRCREATE, UIERROR_SLINKCREATE, UIERROR_HLINKCREATE,
        UIERROR_NOLINKTARGET, UIERROR_NEEDADMIN, UIERROR_ARCBROKEN,
        UIERROR_HEADERBROKEN, UIERROR_MHEADERBROKEN, UIERROR_FHEADERBROKEN,
        UIERROR_SUBHEADERBROKEN, UIERROR_SUBHEADERUNKNOWN,
        UIERROR_SUBHEADERDATABROKEN, UIERROR_RRDAMAGED, UIERROR_UNKNOWNMETHOD,
        UIERROR_UNKNOWNENCMETHOD, UIERROR_RENAMING, UIERROR_NEWERRAR,
        UIERROR_NOTSFX, UIERROR_OLDTOSFX,
        UIERROR_WRONGSFXVER, UIERROR_HEADENCMISMATCH, UIERROR_DICTOUTMEM,
        UIERROR_USESMALLERDICT, UIERROR_MODIFYUNKNOWN, UIERROR_MODIFYOLD,
        UIERROR_MODIFYLOCKED, UIERROR_MODIFYVOLUME, UIERROR_NOTVOLUME,
        UIERROR_NOTFIRSTVOLUME, UIERROR_RECVOLLIMIT, UIERROR_RECVOLDIFFSETS,
        UIERROR_RECVOLALLEXIST, UIERROR_RECVOLFOUND, UIERROR_RECONSTRUCTING,
        UIERROR_RECVOLCANNOTFIX, UIERROR_OPFAILED, UIERROR_UNEXPEOF,
        UIERROR_BADARCHIVE, UIERROR_CMTBROKEN, UIERROR_INVALIDNAME,
        UIERROR_NEWRARFORMAT, UIERROR_NOTSUPPORTED, UIERROR_ENCRNOTSUPPORTED,
        UIERROR_RARZIPONLY, UIERROR_REPAIROLDFORMAT, UIERROR_NOFILESREPAIRED,
        UIERROR_NOFILESTOADD, UIERROR_NOFILESTODELETE, UIERROR_NOFILESTOEXTRACT,
        UIERROR_MISSINGVOL, UIERROR_NEEDPREVVOL, UIERROR_UNKNOWNEXTRA,
        UIERROR_CORRUPTEXTRA, UIERROR_NTFSREQUIRED, UIERROR_ZIPVOLSFX,
        UIERROR_FILERO, UIERROR_TOOLARGESFX, UIERROR_NOZIPSFX, UIERROR_EMAIL,
        UIERROR_ACLGET, UIERROR_ACLBROKEN, UIERROR_ACLUNKNOWN, UIERROR_ACLSET,
        UIERROR_STREAMBROKEN, UIERROR_STREAMUNKNOWN, UIERROR_INCOMPATSWITCH,
        UIERROR_PATHTOOLONG, UIERROR_DIRSCAN, UIERROR_UOWNERGET,
        UIERROR_UOWNERBROKEN, UIERROR_UOWNERGETOWNERID, UIERROR_UOWNERGETGROUPID,
        UIERROR_UOWNERSET, UIERROR_ULINKREAD, UIERROR_ULINKEXIST,
        UIERROR_OPENPRESERVEATIME, UIERROR_READERRTRUNCATED, UIERROR_READERRCOUNT,
        UIERROR_DIRNAMEEXISTS,UIERROR_TRUNCPSW,UIERROR_ADJUSTVALUE,
		
        UIMSG_FIRST,
        UIMSG_STRING, UIMSG_BUILD, UIMSG_RRSEARCH, UIMSG_ANALYZEFILEDATA,
        UIMSG_RRFOUND, UIMSG_RRNOTFOUND, UIMSG_RRDAMAGED, UIMSG_BLOCKSRECOVERED,
        UIMSG_COPYINGDATA, UIMSG_AREADAMAGED, UIMSG_SECTORDAMAGED,
        UIMSG_SECTORRECOVERED, UIMSG_SECTORNOTRECOVERED, UIMSG_FOUND,
        UIMSG_CORRECTINGNAME, UIMSG_BADARCHIVE, UIMSG_CREATING, UIMSG_RENAMING,
        UIMSG_RECVOLCALCCHECKSUM, UIMSG_RECVOLFOUND, UIMSG_RECVOLMISSING,
        UIMSG_MISSINGVOL, UIMSG_RECONSTRUCTING, UIMSG_CHECKSUM, UIMSG_FAT32SIZE,
        UIMSG_SKIPENCARC,
		
        UIWAIT_FIRST,
        UIWAIT_DISKFULLNEXT, UIWAIT_FCREATEERROR, UIWAIT_BADPSW,
		
        UIEVENT_FIRST,
        UIEVENT_SEARCHDUPFILESSTART, UIEVENT_SEARCHDUPFILESEND,
        UIEVENT_CLEARATTRSTART, UIEVENT_CLEARATTRFILE,
        UIEVENT_DELADDEDSTART, UIEVENT_DELADDEDFILE, UIEVENT_FILESFOUND,
        UIEVENT_ERASEDISK, UIEVENT_FILESUMSTART, UIEVENT_FILESUMPROGRESS,
        UIEVENT_FILESUMEND, UIEVENT_PROTECTSTART, UIEVENT_PROTECTEND,
        UIEVENT_TESTADDEDSTART, UIEVENT_TESTADDEDEND, UIEVENT_RRTESTINGSTART,
        UIEVENT_RRTESTINGEND, UIEVENT_NEWARCHIVE, UIEVENT_NEWREVFILE
		};
		
		// Flags for uiAskReplace function.
		enum UIASKREP_FLAGS {
        UIASKREP_F_NORENAME=1,UIASKREP_F_EXCHSRCDEST=2,UIASKREP_F_SHOWNAMEONLY=4
		};
		
		// Codes returned by uiAskReplace. Note that uiAskReplaceEx returns only
		// UIASKREP_R_REPLACE, UIASKREP_R_SKIP and UIASKREP_R_CANCEL codes.
		enum UIASKREP_RESULT {
        UIASKREP_R_REPLACE,UIASKREP_R_SKIP,UIASKREP_R_REPLACEALL,UIASKREP_R_SKIPALL,
        UIASKREP_R_RENAME,UIASKREP_R_RENAMEAUTO,UIASKREP_R_CANCEL,UIASKREP_R_UNUSED
		};
		
		UIASKREP_RESULT uiAskReplace(wchar *Name,size_t MaxNameSize,int64 FileSize,RarTime *FileTime,uint Flags);
		UIASKREP_RESULT uiAskReplaceEx(RAROptions *Cmd,wchar *Name,size_t MaxNameSize,int64 FileSize,RarTime *FileTime,uint Flags);
		
		void uiInit(SOUND_NOTIFY_MODE Sound);
		
		
		void uiStartArchiveExtract(bool Extract,const wchar *ArcName);
		bool uiStartFileExtract(const wchar *FileName,bool Extract,bool Test,bool Skip);
		void uiExtractProgress(int64 CurFileSize,int64 TotalFileSize,int64 CurSize,int64 TotalSize);
		void uiProcessProgress(const char *Command,int64 CurSize,int64 TotalSize);
		
		enum UIPASSWORD_TYPE {UIPASSWORD_GLOBAL,UIPASSWORD_FILE,UIPASSWORD_ARCHIVE};
		bool uiGetPassword(UIPASSWORD_TYPE Type,const wchar *FileName,SecPassword *Password);
		bool uiIsGlobalPasswordSet();
		
		enum UIALARM_TYPE {UIALARM_ERROR, UIALARM_INFO, UIALARM_QUESTION};
		void uiAlarm(UIALARM_TYPE Type);
		
		void uiEolAfterMsg();
		
		bool uiAskNextVolume(wchar *VolName,size_t MaxSize);
		#if !defined(SILENT) && !defined(SFX_MODULE)
		void uiAskRepeatRead(const wchar *FileName,bool &Ignore,bool &All,bool &Retry,bool &Quit);
		#endif
		bool uiAskRepeatWrite(const wchar *FileName,bool DiskFull);
		
		#ifndef SFX_MODULE
		const wchar *uiGetMonthName(int Month);
		#endif
		
		class uiMsgStore
		{
        private:
    static const size_t MAX_MSG = 8;
    const wchar *Str[MAX_MSG];
    uint Num[MAX_MSG];
    uint StrSize,NumSize;
    UIMESSAGE_CODE Code;
        public:
    uiMsgStore(UIMESSAGE_CODE Code)
    {
    // Init arrays in case a caller passes fewer parameters than expected.
    for (uint I=0;I<ASIZE(Str);I++)
Str[I]=L"";
    memset(Num,0,sizeof(Num));
		
    NumSize=StrSize=0;
    this->Code=Code;
    }
    uiMsgStore& operator << (const wchar *s)
    {
    if (StrSize<MAX_MSG)
Str[StrSize++]=s;
    return *this;
    }
    uiMsgStore& operator << (uint n)
    {
    if (NumSize<MAX_MSG)
Num[NumSize++]=n;
    return *this;
    }
		
    void Msg();
		};
		
		
		// Templates recognize usual NULL as integer, not wchar*.
		#define UINULL ((wchar *)NULL)
		
		inline void uiMsg(UIMESSAGE_CODE Code)
		{
        uiMsgStore Store(Code);
        Store.Msg();
		}
		
		template<class T1> void uiMsg(UIMESSAGE_CODE Code,T1 a1)
		{
        uiMsgStore Store(Code);
        Store<<a1;
        Store.Msg();
		}
		
		template<class T1,class T2> void uiMsg(UIMESSAGE_CODE Code,T1 a1,T2 a2)
		{
        uiMsgStore Store(Code);
        Store<<a1<<a2;
        Store.Msg();
		}
		
		template<class T1,class T2,class T3> void uiMsg(UIMESSAGE_CODE code,T1 a1,T2 a2,T3 a3)
		{
        uiMsgStore Store(code);
        Store<<a1<<a2<<a3;
        Store.Msg();
		}
		
		#endif
		