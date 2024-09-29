								--
								-- make_vala.lua
								-- Generate a Vala project makefile.
								--
								
							premake.make.vala = { }
							premake.make.makefile_ignore = false
								
							local vala = premake.make.vala
							local make = premake.make
								
							function premake.make_vala(prj)
								
						-- create a shortcut to the compiler interface
						local valac = premake.gettool(prj)
								
						-- build a list of supported target platforms that also includes a generic build
						local platforms = premake.filterplatforms(prj.solution, valac.platforms, "Native")
								
						-- output build configurations
						premake.gmake_vala_header(prj, valac, platforms)
								
						-- write configuration blocks
						for _, platform in ipairs(platforms) do
					for cfg in premake.eachconfig(prj, platform) do
				premake.gmake_valac_config(prj, cfg, valac)
					end
						end
								
						-- list object directories
						local objdirs = {}
						local additionalobjdirs = {}
						for _, file in ipairs(prj.allfiles) do
					if path.issourcefile(file) or path.isgresource(file) then
				objdirs[_MAKE.esc(path.getdirectory(path.trimdots(file)))] = 1
					end
						end
								
						_p('OBJDIRS := \\')
						_p('\t$(OBJDIR) \\')
						for dir, _ in iter.sortByKeys(objdirs) do
					_p('\t$(OBJDIR)/%s \\', dir)
						end
						for dir, _ in iter.sortByKeys(additionalobjdirs) do
					_p('\t%s \\', dir)
						end
						_p('')
								
						-- main build rule(s)
						_p('.PHONY: clean prebuild prelink')
						_p('')
								
						_p('all: $(OBJDIRS) $(TARGETDIR) prebuild prelink $(TARGET)')
						_p('\t@:')
						_p('')
								
						-- target build rule
						_p('$(TARGET): $(OBJECTS) | $(TARGETDIR)')
						_p('\t@echo Linking %s', prj.name)
						_p('\t$(SILENT) $(LINKCMD)')
								
						_p('\t$(POSTBUILDCMDS)')
						_p('')
								
						-- Create destination directories. Can't use $@ for this because it loses the
						-- escaping, causing issues with spaces and parenthesis
						_p('$(TARGETDIR):')
						premake.make_mkdirrule("$(TARGETDIR)")
								
						_p('$(OBJDIRS):')
						if (not prj.solution.messageskip) or (not table.contains(prj.solution.messageskip, "SkipCreatingMessage")) then
						_p('\t@echo Creating $(@)')
						end
						_p('\t-$(call MKDIR,$@)')
						_p('')
								
						-- clean target
						_p('clean:')
						if (not prj.solution.messageskip) or (not table.contains(prj.solution.messageskip, "SkipCleaningMessage")) then
					_p('\t@echo Cleaning %s', prj.name)
						end
						_p('ifeq (posix,$(SHELLTYPE))')
						_p('\t$(SILENT) rm -f  $(TARGET)')
						_p('else')
						_p('\t$(SILENT) if exist $(subst /,\\\\,$(TARGET)) del $(subst /,\\\\,$(TARGET))')
						_p('endif')
						_p('')
								
						-- custom build step targets
						_p('prebuild:')
						_p('\t$(PREBUILDCMDS)')
						_p('')
								
						_p('prelink:')
						_p('\t$(PRELINKCMDS)')
						_p('')
								
						vala.fileRules(prj, valac)
							end
								
								
								
								--
								-- Write the makefile header
								--
								
							function premake.gmake_vala_header(prj, valac, platforms)
						_p('# %s project makefile autogenerated by GENie', premake.action.current().shortname)
						_p('')
								
						_p('.SUFFIXES:') -- Delete the default suffix rules.
						_p('')
								
						-- set up the environment
						_p('ifndef config')
						_p('  config=%s', _MAKE.esc(premake.getconfigname(prj.solution.configurations[1], platforms[1], true)))
						_p('endif')
						_p('')
								
						_p('ifndef verbose')
						_p('  SILENT = @')
						_p('endif')
						_p('')
								
						-- identify the shell type
						_p('SHELLTYPE := msdos')
						_p('ifeq (,$(ComSpec)$(COMSPEC))')
						_p('  SHELLTYPE := posix')
						_p('endif')
						_p('ifeq (/bin,$(findstring /bin,$(SHELL)))')
						_p('  SHELLTYPE := posix')
						_p('endif')
						_p('ifeq (/bin,$(findstring /bin,$(MAKESHELL)))')
						_p('  SHELLTYPE := posix')
						_p('endif')
						_p('')
								
						_p('ifeq (posix,$(SHELLTYPE))')
						_p('  MKDIR = $(SILENT) mkdir -p "$(1)"')
						_p('  COPY  = $(SILENT) cp -fR "$(1)" "$(2)"')
						_p('  RM    = $(SILENT) rm -f "$(1)"')
						_p('else')
						_p('  MKDIR = $(SILENT) mkdir "$(subst /,\\\\,$(1))" 2> nul || exit 0')
						_p('  COPY  = $(SILENT) copy /Y "$(subst /,\\\\,$(1))" "$(subst /,\\\\,$(2))"')
						_p('  RM    = $(SILENT) del /F "$(subst /,\\\\,$(1))" 2> nul || exit 0')
						_p('endif')
						_p('')
								
						_p('VALAC  = %s', valac.valac)
						_p('CC     = %s', valac.cc)
						_p('GLIBRC = %s', valac.glibrc)
						_p('')
								
						if (not premake.make.makefile_ignore) then
					_p('MAKEFILE = %s', _MAKE.getmakefilename(prj, true))
					_p('')
						end
							end
								
								
								
								--
								-- Write a block of configuration settings.
								--
								
							local function is_excluded(prj, cfg, file)
						if table.icontains(prj.excludes, file) then
					return true
						end
								
						if table.icontains(cfg.excludes, file) then
					return true
						end
								
						return false
							end
								
							function premake.gmake_valac_config(prj, cfg, valac)
								
						_p('ifeq ($(config),%s)', _MAKE.esc(cfg.shortname))
								
						_p('  BASEDIR    = %s', _MAKE.esc(path.getrelative(cfg.location, _WORKING_DIR)))
						_p('  OBJDIR     = %s', _MAKE.esc(cfg.objectsdir))
						_p('  TARGETDIR  = %s', _MAKE.esc(cfg.buildtarget.directory))
						_p('  TARGET     = $(TARGETDIR)/%s', _MAKE.esc(cfg.buildtarget.name))
						_p('  DEFINES    +=%s', make.list(valac.getdefines(cfg.defines)))
						_p('  VAPIDIRS   +=%s', make.list(valac.getvapidirs(cfg.vapidirs)))
						_p('  PKGS       +=%s', make.list(valac.getlinks(cfg.links)))
						_p('  FLAGS      += $(DEFINES) $(VAPIDIRS) $(PKGS)%s', make.list(table.join(valac.getvalaflags(cfg), cfg.buildoptions_vala)))
						_p('  ALL_LDFLAGS+= $(LDFLAGS)%s', make.list(table.join(cfg.linkoptions)))
						_p('  LINKOBJS    = %s', "$(OBJECTS)")
						_p('  ALL_CFLAGS += $(CFLAGS) $(ARCH)%s', make.list(table.join(cfg.buildoptions, cfg.buildoptions_c, valac.getvalaccflags(cfg))))
						_p('  LINKCMD     = $(CC) -o $(TARGET) $(LINKOBJS) $(ARCH) $(ALL_LDFLAGS)');
								
						table.sort(cfg.files)
								
						-- add objects for compilation, and remove any that are excluded per config.
						_p('  OBJECTS := \\')
						for _, file in ipairs(cfg.files) do
					if path.issourcefile(file) or path.isgresource(file) then
				if not is_excluded(prj, cfg, file) then
			_p('\t$(OBJDIR)/%s.o \\'
		, _MAKE.esc(path.trimdots(path.removeext(file)))
		)
				end
					end
						end
						_p('')
								
						_p('  SOURCES := \\')
						for _, file in ipairs(cfg.files) do
					if path.issourcefile(file) and path.isvalafile(file) then
				if not is_excluded(prj, cfg, file) then
			_p('\t%s \\', _MAKE.esc(file))
				end
					end
						end
						_p('')
								
						_p('  GRESOURCES := \\')
						for _, file in ipairs(cfg.files) do
					if path.isgresource(file) then
				if not is_excluded(prj, cfg, file) then
			_p('\t%s \\', _MAKE.esc(file))
				end
					end
						end
						_p('')
								
						_p('  define PREBUILDCMDS')
						if #cfg.prebuildcommands > 0 then
					_p('\t@echo Running pre-build commands')
					_p('\t%s', table.implode(cfg.prebuildcommands, "", "", "\n\t"))
						end
						_p('  endef')
								
						_p('  define PRELINKCMDS')
						if #cfg.prelinkcommands > 0 then
					_p('\t@echo Running pre-link commands')
					_p('\t%s', table.implode(cfg.prelinkcommands, "", "", "\n\t"))
						end
						_p('  endef')
								
						_p('  define POSTBUILDCMDS')
						if #cfg.postbuildcommands > 0 then
					_p('\t@echo Running post-build commands')
					_p('\t%s', table.implode(cfg.postbuildcommands, "", "", "\n\t"))
						end
						_p('  endef')
								
						_p('endif')
						_p('')
							end
								
								
								
								--
								-- Build command for a single file.
								--
								
							function vala.fileRules(prj, cc)
						local platforms = premake.filterplatforms(prj.solution, cc.platforms, "Native")
								
						_p('ifneq (,$(OBJRESP))')
						_p('$(OBJRESP): $(OBJECTS) | $(TARGETDIR) $(OBJDIRS)')
						_p('\t$(SILENT) echo $^')
						_p('\t$(SILENT) echo $^ > $@')
						_p('endif')
						_p('')
								
						_p('ifneq (,$(LDRESP))')
						_p('$(LDRESP): $(LDDEPS) | $(TARGETDIR) $(OBJDIRS)')
						_p('\t$(SILENT) echo $^')
						_p('\t$(SILENT) echo $^ > $@')
						_p('endif')
						_p('')
								
						local pattern_targets = ""
						table.sort(prj.allfiles)
						for _, file in ipairs(prj.allfiles or {}) do
					if path.issourcefile(file) or path.isgresource(file) then
				if path.isvalafile(file) or path.iscfile(file) or path.isgresource(file) then
			if path.isvalafile(file) or path.isgresource(file) then
		_p('$(OBJDIR)/%s.o: $(OBJDIR)/%s.c $(GCH) $(MAKEFILE) | $(OBJDIR)/%s'
	, _MAKE.esc(path.trimdots(path.removeext(file)))
	, _MAKE.esc(path.trimdots(path.removeext(file)))
	, _MAKE.esc(path.getdirectory(path.trimdots(file)))
	)
		if not path.isgresource(file) then
	pattern_targets = pattern_targets
.. "$(OBJDIR)/"
.. _MAKE.esc(path.trimdots(path.removeext(file)))
.. ".% \\\n" -- Pattern rule: https://stackoverflow.com/a/3077254
		end
			else
		_p('$(OBJDIR)/%s.o: %s $(GCH) $(MAKEFILE) | $(OBJDIR)/%s'
	, _MAKE.esc(path.trimdots(path.removeext(file)))
	, file
	, _MAKE.esc(path.getdirectory(path.trimdots(file)))
	)
			end
			if prj.msgcompile then
		_p('\t@echo ' .. prj.msgcompile)
			else
		_p('\t@echo $(notdir $<)')
			end
			_p('\t$(SILENT) %s $(FORCE_INCLUDE) -o "$@" -c "$<"'
		, "$(CC) $(ALL_CFLAGS)"
		, _MAKE.esc(path.getbasename(file))
		)
			for _, task in ipairs(prj.postcompiletasks or {}) do
		_p('\t$(SILENT) %s', task)
		_p('')
			end
			_p('')
								
			if path.isgresource(file) then
		_p('$(OBJDIR)/%s.c: %s $(GCH) $(MAKEFILE) | $(OBJDIR)/%s'
	, _MAKE.esc(path.trimdots(path.removeext(file)))
	, _MAKE.esc(file)
	, _MAKE.esc(path.getdirectory(path.trimdots(file)))
	)
		if prj.msgcompile then
	_p('\t@echo ' .. prj.msgcompile)
		else
	_p('\t@echo $(notdir $<)')
		end
		_p('\t$(SILENT) %s "$<" --target="$@" --sourcedir="%s" --generate'
	, "$(GLIBRC)"
	, _MAKE.esc(path.getdirectory(file))
	)
		for _, task in ipairs(prj.postcompiletasks or {}) do
	_p('\t$(SILENT) %s', task)
	_p('')
		end
			end
								
			_p('')
				end
					end
						end
								
						if pattern_targets ~= "" then
					-- Generate .c from corresponding .vala
					_p('%s: $(SOURCES) $(GRESOURCES) $(GCH) $(MAKEFILE)', pattern_targets)
					if prj.msgcompile then
				_p('\t@echo ' .. prj.msgcompile)
					else
				_p('\t@echo [GEN] valac')
					end
					_p('\t$(SILENT) %s $(SOURCES) --directory $(OBJDIR) --basedir $(BASEDIR) --gresources=$(GRESOURCES) -C > /dev/null'
				, "$(VALAC) $(FLAGS)"
				)
					for _, task in ipairs(prj.postcompiletasks or {}) do
				_p('\t$(SILENT) %s', task)
				_p('')
					end
						end
							end
								