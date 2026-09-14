#!/bin/sh
set -eu

assert_game()
{
    test_name=$1
    test_input=$2
    expected_text=$3

    if game_output=$(printf '%s' "$test_input" | ./dungeon map.txt 2>&1)
    then
        case "$game_output" in
            *"$expected_text"*)
                printf 'PASS: %s\n' "$test_name"
                ;;
            *)
                printf 'FAIL: %s (expected "%s")\n' \
                    "$test_name" "$expected_text" >&2
                return 1
                ;;
        esac
    else
        printf 'FAIL: %s (game returned an error)\n' "$test_name" >&2
        return 1
    fi
}

if no_arg_output=$(./dungeon 2>&1)
then
    printf 'FAIL: missing argument should return an error\n' >&2
    exit 1
else
    case "$no_arg_output" in
        *"Usage:"*)
            printf 'PASS: command-line argument validation\n'
            ;;
        *)
            printf 'FAIL: missing argument message\n' >&2
            exit 1
            ;;
    esac
fi

if missing_map_output=$(./dungeon map-does-not-exist.txt 2>&1)
then
    printf 'FAIL: missing map should return an error\n' >&2
    exit 1
else
    case "$missing_map_output" in
        *"could not open map file"*)
            printf 'PASS: missing map validation\n'
            ;;
        *)
            printf 'FAIL: missing map message\n' >&2
            exit 1
            ;;
    esac
fi

win_path='dddwwwwwddwwwddddddddddddaaaaaddddd'
assert_game 'winning route' "$win_path" 'You win!'
assert_game 'undo then winning route' "du$win_path" 'You win!'
assert_game 'invalid key is ignored' "x$win_path" 'You win!'
assert_game 'danger-zone loss' 'dddwwwwwdddd' 'You lose!'
assert_game 'five-grid fall loss' 'dddwwwwwaaa' 'You lose!'

printf 'All 7 tests passed.\n'
