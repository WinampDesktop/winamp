Specifies shell commands to run after the source files have been compiled, but before the link step.

```lua
prelinkcommands { "commands" }
```

### Parameters ###

`commands` is one or more shell commands. These commands will be passed to the shell exactly as entered, including path separators and the like.

### Applies To ###

Project configurations.

### Availability ###

Premake 4.4 or later.

### Examples ###

```lua
filter { "system:arch" }
   prelinkcommands { "copy default.config bin\\project.config" }

filter { "not system:arch" }
   prelinkcommands { "cp default.config bin/project.config" }
```

### See Also ###

 * [prebuildcommands](prebuildcommands.md)
 * [postbuildcommands](postbuildcommands.md)
