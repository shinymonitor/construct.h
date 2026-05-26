<div align="center">
    <img src="assets/LOGO.png", width="200"/>
    <h1>construct.h</h1>
</div>

A single-header C library for building projects with minimal dependencies using build recipes in C itself. Provides dynamic arrays, string builder and view, temp functions, cross-platform file handling, and CONSTRUCT build system (Make-style) with auto dependency scanning.

Inspired by and simplified from [nob.h](https://github.com/tsoding/nob.h).

## Features

- Dynamic arrays with automatic growth/shrink
- String builders and string views
- Cross-platform file operations
- Temporary string functions
- Logging utilities
- Make like build with dependency scanning (CONSTRUCT)
- Header-only library

## Why construct.h?

Traditional build systems have drawbacks:
- Make: Limited language, platform-specific syntax
- CMake: Complex DSL, steep learning curve  
- Shell scripts: Not cross-platform, poor error handling

construct.h lets you write build scripts in C with:
- Full C language power
- Cross-platform by default
- Incremental builds

## CONSTRUCT Build System

The build system tracks dependencies and rebuilds only when needed (Like Make):

```c
    ConstructRules rules = {0};

    ConstructRule rule = {SA("build/project"), SA("src/main.c"), 0};
    command_append(&rule.command,"gcc", "src/main.c", "-o", "build/project", "-Wall -Wextra -Werror -O3");
    da_append(&rules, rule);

    if (!run_construct(&rules, true)) {print_log(ERROR, "Build failed"); return 1;}
    print_log(INFO, "Build finished");

    return 0;
```

The build system:
- Checks if targets exist
- Compares modification times
- Runs commands only when targets are missing or out of date
- Handles dependency chains automatically
- Does dependency scanning in the source

