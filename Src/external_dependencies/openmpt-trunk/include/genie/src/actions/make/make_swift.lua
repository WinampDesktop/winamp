			--
			-- make_swift.lua
			-- Generate a Swift project makefile.
			--
			
			local make = premake.make
			local swift = { }
			
			function premake.make_swift(prj)
		local tool = premake.gettool(prj)
			
		-- build a list of supported target platforms that also includes a generic build
		local platforms = premake.filterplatforms(prj.solution, tool.platforms, "Native")
			
		_p('# %s project makefile autogenerated by GENie', premake.action.current().shortname)
		_p('')
			
		_p('.SUFFIXES:') -- Delete the default suffix rules.
		_p('')
			
		-- set up the environment
		_p('ifndef config')
		_p(1, 'config=%s', _MAKE.esc(premake.getconfigname(prj.solution.configurations[1], platforms[1], true)))
		_p('endif')
		_p('')
			
		_p('ifndef verbose')
		_p(1, 'SILENT = @')
		_p('endif')
		_p('')
			
		-- identify the shell type
		_p('SHELLTYPE := msdos')
		_p('ifeq (,$(ComSpec)$(COMSPEC))')
		_p(1, 'SHELLTYPE := posix')
		_p('endif')
		_p('ifeq (/bin,$(findstring /bin,$(SHELL)))')
		_p(1, 'SHELLTYPE := posix')
		_p('endif')
		_p('ifeq (/bin,$(findstring /bin,$(MAKESHELL)))')
		_p(1, 'SHELLTYPE := posix')
		_p('endif')
		_p('')
			
		_p('ifeq (posix,$(SHELLTYPE))')
		_p(1, 'MKDIR = $(SILENT) mkdir -p "$(1)"')
		_p(1, 'COPY  = $(SILENT) cp -fR "$(1)" "$(2)"')
		_p(1, 'RM    = $(SILENT) rm -f "$(1)"')
		_p('else')
		_p(1, 'MKDIR = $(SILENT) mkdir "$(subst /,\\\\,$(1))" 2> nul || exit 0')
		_p(1, 'COPY  = $(SILENT) copy /Y "$(subst /,\\\\,$(1))" "$(subst /,\\\\,$(2))"')
		_p(1, 'RM    = $(SILENT) del /F "$(subst /,\\\\,$(1))" 2> nul || exit 0')
		_p('endif')
		_p('')
			
		_p('SWIFTC = %s', tool.swift)
		_p('SWIFTLINK = %s', tool.swiftc)
		_p('DSYMUTIL = %s', tool.dsymutil)
		_p('AR = %s', tool.ar)
		_p('')
			
		-- write configuration blocks
		for _, platform in ipairs(platforms) do
	for cfg in premake.eachconfig(prj, platform) do
swift.generate_config(prj, cfg, tool)
	end
		end
			
		_p('.PHONY: ')
		_p('')
		_p('all: $(WORK_DIRS) $(TARGET)')
		_p('')
			
		_p('$(WORK_DIRS):')
		_p(1, '$(SILENT) $(call MKDIR,$@)')
		_p('')
			
		_p('SOURCES := \\')
		for _, file in ipairs(prj.files) do
	if path.isswiftfile(file) then
_p(1, '%s \\', _MAKE.esc(file))
	end
		end
		_p('')
			
		local objfiles = {}
		_p('OBJECTS_WITNESS := $(OBJDIR)/build.stamp')
		_p('OBJECTS := \\')
		for _, file in ipairs(prj.files) do
	if path.isswiftfile(file) then
local objname = _MAKE.esc(swift.objectname(file))
table.insert(objfiles, objname)
_p(1, '%s \\', objname)
	end
		end
		_p('')
			
		swift.file_rules(prj, objfiles)
		swift.linker(prj, tool)
		swift.generate_clean(prj)
			end
			
			function swift.objectname(file)
		return path.join("$(OBJDIR)", path.getname(file)..".o")
			end
			
			function swift.file_rules(prj, objfiles)
	-- use a witness file per recommendation for handling multiple outputs
	-- see https://www.gnu.org/software/automake/manual/html_node/Multiple-Outputs.html
		_p('$(OBJECTS_WITNESS): $(SOURCES)')
		_p(1, "@rm -f $(OBJDIR)/data.tmp")
		_p(1, "@touch $(OBJDIR)/data.tmp")
		_p(1, "$(SILENT) $(SWIFTC) -frontend -c $(SOURCES) -enable-objc-interop $(SDK) -I $(OUT_DIR) $(SWIFTC_FLAGS) -module-cache-path $(MODULECACHE_DIR) -D SWIFT_PACKAGE $(MODULE_MAPS) -emit-module-doc-path $(OUT_DIR)/$(MODULE_NAME).swiftdoc -module-name $(MODULE_NAME) -emit-module-path $(OUT_DIR)/$(MODULE_NAME).swiftmodule -num-threads 8 %s", table.arglist("-o", objfiles))
		_p(1, "@mv -f $(OBJDIR)/data.tmp $(OBJECTS_WITNESS)")
		_p('')
			
		-- posix only for now
		_p('$(OBJECTS): $(OBJECTS_WITNESS)')
		_p(1, '@if test -f $@; then :; else \\')
		_p(2, 'rm -f $(OBJECTS_WITNESS); \\')
		_p(2, '$(MAKE) $(AM_MAKEFLAGS) $(OBJECTS_WITNESS); \\')
		_p(1, 'fi')
		_p('')
			end
			
			function swift.linker(prj, tool)
		local lddeps = make.list(premake.getlinks(prj, "siblings", "fullpath"))
			
		if prj.kind == "StaticLib" then
	_p("$(TARGET): $(OBJECTS) %s ", lddeps)
	_p(1, "$(SILENT) $(AR) cr $(AR_FLAGS) $@ $(OBJECTS) %s", (os.is("MacOSX") and " 2>&1 > /dev/null | sed -e '/.o) has no symbols$$/d'" or ""))
		else
	_p("$(TARGET): $(OBJECTS) $(LDDEPS)", lddeps)
	_p(1, "$(SILENT) $(SWIFTLINK) $(SDK) -L $(OUT_DIR) -o $@ $(SWIFTLINK_FLAGS) $(LD_FLAGS) $(OBJECTS)")
	_p("ifdef SYMBOLS")
	_p(1, "$(SILENT) $(DSYMUTIL) $(TARGET) -o $(SYMBOLS)")
	_p("endif")
		end
		_p('')
			end
			
			function swift.generate_clean(prj)
		_p('clean:')
		if (not prj.solution.messageskip) or (not table.contains(prj.solution.messageskip, "SkipCleaningMessage")) then
	_p('\t@echo Cleaning %s', prj.name)
		end
		_p('ifeq (posix,$(SHELLTYPE))')
		_p('\t$(SILENT) rm -f  $(TARGET)')
		_p('\t$(SILENT) rm -rf $(OBJDIR)')
		_p('\t$(SILENT) rm -rf $(SYMBOLS)')
		_p('\t$(SILENT) rm -rf $(MODULECACHE_DIR)')
		_p('else')
		_p('\t$(SILENT) if exist $(subst /,\\\\,$(TARGET)) del $(subst /,\\\\,$(TARGET))')
		_p('\t$(SILENT) if exist $(subst /,\\\\,$(OBJDIR)) rmdir /s /q $(subst /,\\\\,$(OBJDIR))')
		_p('\t$(SILENT) if exist $(subst /,\\\\,$(SYMBOLS)) rmdir /s /q $(subst /,\\\\,$(SYMBOLS))')
		_p('\t$(SILENT) if exist $(subst /,\\\\,$(MODULECACHE_DIR)) rmdir /s /q $(subst /,\\\\,$(MODULECACHE_DIR))')
		_p('endif')
		_p('')
			end
			
			function swift.generate_config(prj, cfg, tool)
		_p('ifeq ($(config),%s)', _MAKE.esc(cfg.shortname))
			
		_p(1, "OUT_DIR = %s", cfg.buildtarget.directory)
            _p(1, "MODULECACHE_DIR = $(OUT_DIR)/ModuleCache")
		_p(1, "TARGET = $(OUT_DIR)/%s", _MAKE.esc(cfg.buildtarget.name))
		local objdir = path.join(cfg.objectsdir, prj.name .. ".build")
		_p(1, "OBJDIR = %s", objdir)
		_p(1, "MODULE_NAME = %s", prj.name)
		_p(1, "MODULE_MAPS = %s", make.list(tool.getmodulemaps(cfg)))
		_p(1, "SWIFTC_FLAGS = %s", make.list(tool.getswiftcflags(cfg)))
		_p(1, "SWIFTLINK_FLAGS = %s", make.list(tool.getswiftlinkflags(cfg)))
		_p(1, "AR_FLAGS = %s", make.list(tool.getarchiveflags(cfg, cfg, false)))
		_p(1, "LD_FLAGS = %s", make.list(tool.getldflags(cfg)))
		_p(1, "LDDEPS = %s", make.list(premake.getlinks(cfg, "siblings", "fullpath")))
		if cfg.flags.Symbols then
	_p(1, "SYMBOLS = $(TARGET).dSYM")
		end
			
		local sdk = tool.get_sdk_path(cfg)
		if sdk then
	_p(1, "TOOLCHAIN_PATH = %s", tool.get_toolchain_path(cfg))
	_p(1, "SDK_PATH = %s", sdk)
	_p(1, "PLATFORM_PATH = %s", tool.get_sdk_platform_path(cfg))
	_p(1, "SDK = -sdk $(SDK_PATH)")
		else
	_p(1, "SDK_PATH =")
	_p(1, "SDK =")
		end
			
		_p(1,'WORK_DIRS = $(OUT_DIR) $(OBJDIR)')
			
		_p('endif')
		_p('')
			end
			