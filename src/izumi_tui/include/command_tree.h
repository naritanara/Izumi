/*
 * This file is part of Izumi.
 *
 * Izumi is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Izumi is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Izumi. If not, see <https://www.gnu.org/licenses/>.
 */
 
#ifndef COMMAND_TREE_H
#define COMMAND_TREE_H

#include <stdbool.h>
#include <stdio.h>

#include "window.h"

typedef struct CommandTree_s CommandTree;

bool run_command(ApplicationData *app_data);

enum CommandType_e {
    COMMAND_TYPE_ARGLIST,
    COMMAND_TYPE_FIXED_ARGLIST,
    COMMAND_TYPE_NO_ARGS,
    COMMAND_TYPE_SUBCOMMAND,
    COMMAND_TYPE_ALIAS,
};

typedef enum CommandType_e CommandType;

typedef bool(CommandArglistCallback)(ApplicationData *app_data, int argc, const char * argv[]);

struct CommandArglist_s {
    CommandArglistCallback * callback;
};

typedef struct CommandArglist_s CommandArglist;

typedef bool(CommandFixedArglistCallback)(ApplicationData *app_data, const char * argv[]);

struct CommandFixedArglist_s {
    int argc;
    CommandFixedArglistCallback * callback;
};

typedef struct CommandFixedArglist_s CommandFixedArglist;

typedef bool(CommandNoArgsCallback)(ApplicationData *app_data);

struct CommandNoArgs_s {
    CommandNoArgsCallback * callback;
};

typedef struct CommandNoArgs_s CommandNoArgs;

struct CommandSubcommand_s {
    const CommandTree * subcommand_tree;
};

typedef struct CommandSubcommand_s CommandSubcommand;

struct CommandAlias_s {
    const char * real_cmd;
};

typedef struct CommandAlias_s CommandAlias;

struct Command_s {
    const char * cmd;
    CommandType type;
    union {
        CommandArglist arglist;
        CommandFixedArglist fixed_arglist;
        CommandNoArgs no_args;
        CommandSubcommand subcommand;
        CommandAlias alias;
    };
};

typedef struct Command_s Command;

struct CommandTree_s {
    const Command * commands;
    size_t commands_length;
};

#define CMD_ARGLIST(cmd, callback)             { cmd, COMMAND_TYPE_ARGLIST, { .arglist = { callback } } }
#define CMD_FIXED_ARGLIST(cmd, argc, callback) { cmd, COMMAND_TYPE_FIXED_ARGLIST, { .fixed_arglist = { argc, callback } } }
#define CMD_NO_ARGS(cmd, callback)             { cmd, COMMAND_TYPE_NO_ARGS, { .no_args = { callback } } }
#define CMD_SUBCOMMAND(cmd, subcommands)       { cmd, COMMAND_TYPE_SUBCOMMAND, { .subcommand = { subcommands } } }
#define CMD_ALIAS(cmd, real_cmd)               { cmd, COMMAND_TYPE_ALIAS, { .alias = { real_cmd } } }

#define MAKE_COMMAND_TREE(name, ...) \
    const Command name##__CMDLIST[] = { __VA_ARGS__ }; \
    const CommandTree name = { name##__CMDLIST, sizeof(name##__CMDLIST) / sizeof(Command) };

#endif
