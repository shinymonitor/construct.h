<div align="center">
    <img src="assets/LOGO.png", width="200"/>
    <h1>construct.h</h1>
</div>

A single-header C library for building projects with minimal dependencies using build recipes in C itself. Provides dynamic arrays, string manipulation, process management, and CONSTRUCT build system (similar to Make).

Inspired by and simplified from [nob.h](https://github.com/tsoding/nob.h), with additional features for incremental builds.

## Features

- Dynamic arrays with automatic growth/shrink
- String builders and string views
- Cross-platform file operations
- Process and command execution (sync/async)
- Build system with dependency tracking (CONSTRUCT)
- Temporary string allocation
- Logging utilities
- Header-only library

## Why construct.h?

Traditional build systems have drawbacks:
- Make: Limited language, platform-specific syntax
- CMake: Complex DSL, steep learning curve  
- Shell scripts: Not cross-platform, poor error handling

construct.h lets you write build scripts in C with:
- Full C language power
- Cross-platform by default
- Incremental builds (CONSTRUCT system)
- Type safety and IDE support

## Comparison with nob.h

construct.h is a simplified and extended version of nob.h:

Similarities:
- Dynamic arrays
- Command building and execution
- String utilities
- Process management

Differences:
- Added CONSTRUCT build system for incremental builds
- Simplified API
- Less LOC and better code structure
- Additional utilities (See additional features in Dynamic Arrays, Misc, Short cicuiting, String View)


## Quick Start

```c
#include "construct.h"

int main(void) {
    Command cmd = {0};
    command_append(&cmd, "gcc", "-o", "program", "main.c");
    if (!command_run(&cmd)) {
        print_log(ERROR, "Compilation failed");
        return 1;
    }
    print_log(INFO, "Build successful");
    return 0;
}
```

Compile your build script:
```bash
gcc -o build build.c
./build
```
or if you have `nct` installed, just replace build.h 

## Core Components

### Dynamic Arrays

```c
da_define(int, IntArray);
IntArray arr = {0};

da_append(&arr, 42);
da_append(&arr, 123);

printf("Count: %zu\n", arr.count);
da_free(&arr);
```

### String Builder

```c
StringBuilder sb = {0};
sb_append_str(&sb, "Hello ");
sb_append_str(&sb, "World");
sb_append_null(&sb);

printf("%s\n", sb.items);
sb_free(&sb);
```

### Commands

```c
Command cmd = {0};
command_append(&cmd, "clang", "-Wall", "-o", "output", "input.c");

// Synchronous
command_run(&cmd);

// Asynchronous
Procs procs = {0};
command_run(&cmd, .async = &procs, .max_procs = 4);
procs_wait(&procs);
```

### File Operations

```c
if (!file_exists("config.txt")) {
    print_log(WARNING, "Config not found");
}

if (is_newer("main.c", "main.o")) {
    // Recompile
}

DABytes content = {0};
read_entire_file("data.bin", &content);
// Use content.items and content.count
da_free(&content);
```

### CONSTRUCT Build System

The build system tracks dependencies and rebuilds only when needed:

```c
ConstructRules rules = {0};

// Rule: compile main.c -> main.o
{
    ConstructRule rule = {0};
    rule.targets = (char*[]){"main.o"};
    rule.targets_count = 1;
    rule.dependencies = (char*[]){"main.c"};
    rule.dependencies_count = 1;
    
    command_append(&rule.command_with_options.command, 
                   "gcc", "-c", "main.c", "-o", "main.o");
    
    da_append(&rules, rule);
}

// Rule: link main.o -> program
{
    ConstructRule rule = {0};
    rule.targets = (char*[]){"program"};
    rule.targets_count = 1;
    rule.dependencies = (char*[]){"main.o"};
    rule.dependencies_count = 1;
    
    command_append(&rule.command_with_options.command,
                   "gcc", "main.o", "-o", "program");
    
    da_append(&rules, rule);
}

// Run build system
if (!run_construct(&rules)) {
    print_log(ERROR, "Build failed");
    return 1;
}
```

The build system:
- Checks if targets exist
- Compares modification times
- Runs commands only when targets are missing or out of date
- Handles dependency chains automatically


## Platform Support

- Linux/Unix (tested on Linux, macOS)
- Windows (MinGW, MSVC)

All file operations, path handling, and process management work cross-platform.
