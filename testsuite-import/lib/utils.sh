#!/bin/bash

MKFS_EXT4_BIN=/usr/sbin/mkfs.ext4
MKFS_BTRFS_BIN=/usr/sbin/mkfs.btrfs

WIPEFS_BIN=/usr/sbin/wipefs

VGCREATE_BIN=/usr/sbin/vgcreate
LVCREATE_BIN=/usr/sbin/lvcreate
LVREMOVE_BIN=/usr/sbin/lvremove
PVREMOVE_BIN=/usr/sbin/pvremove
VGREMOVE_BIN=/usr/sbin/vgremove

function pdebug() {
	if [ -n "$DEBUG" ]; then
		if [ "$1" == "-n" ]; then
			shift
			echo -n "$@"
		else
			echo "$@"
		fi
	fi
}

function lib_checkparams() {

	if [  "$COMMAND" != "setup" -a "$COMMAND" != "cleanup" ]; then
		echo "--command parmeter is invalid" >&2
		return 100
	fi
	pdebug "COMMAND=$COMMAND"

	if [ -z "$FS" -a $COMMAND != "cleanup" ]; then
		echo "--filesystem parameter is mandatory" >&2
		return 100
	fi
	pdebug "FS=$FS"

	if [ "$PV_COUNT" -lt $MIN_PV_COUNT -a $COMMAND != "cleanup" ]; then
		echo "You have to specify at least $MIN_PV_COUNT block device(s)" >&2
		return 100
	fi
	pdebug "PV_COUNT=$PV_COUNT"
}

function cleanup() {
	trap - ERR
	test -n "$REG_CLEANUP" && {
		pdebug "entering cleanup function: $REG_CLEANUP"
		"$REG_CLEANUP"
	}
}

function register_cleanup() {
	type "$1" | grep -q "function" && {
		pdebug "registering cleanup function: '$1'"
		REG_CLEANUP="$1"
	}
}

function setup_fs_ext4() {
	$MKFS_EXT4_BIN -q -E nodiscard "$1"
}

function setup_fs_btrfs() {
	$MKFS_BTRFS_BIN -K "$1" > /dev/null
}

# setup_fs_xfs

function mount_test_volume() {
	pdebug "-n" "entering mount test volume..."

	if [ $# -lt 2 ]; then
		pdebug "ERR"
		pdebug "usage mount_test_volume() <device> <mountpoint> [<mountoptions>]"
		return 3
	fi

	# add test for third optional parameter w/ mount options
	local OPTIONS=nodev,noatime,nodiratime,nosuid,noexec
	mount -o "$OPTIONS" "$1" "$2"
	pdebug "done"
}

STACKTRACE() {
	trap - ERR
        local i=0

        echo "## - $0:${BASH_LINENO[0]}"
        while FUNC=${FUNCNAME[$i]}; test "$FUNC" != "main"; do
                echo "## $i ${FUNC}() called from ${BASH_SOURCE[$i]}:${BASH_LINENO[$i]}"
                i=$(($i + 1))
        done
}

set -eE

trap 'ret=$?; set +vxeE; STACKTRACE; cleanup; exit $ret' ERR
