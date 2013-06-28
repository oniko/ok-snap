#!/bin/bash

TEST_VG_0=vg_test
TEST_VG_1=vg_test_2

TEST_POOL_0=lv_test_pool_1
TEST_POOL_1=lv_test_pool_2

TEST_ORIGIN_0=lv_test_thin_1
TEST_ORIGIN_1=lv_test_thin_2

TEST_VOLUME=/testsuite-import

MIN_PV_COUNT=2

. lib/utils.sh

function cleanup() {
	umount -f $TEST_VOLUME
	local l_pv_count=$[PV_COUNT-1]

	rmdir $TEST_VOLUME

	while [ "$l_pv_count" -ge 0 ]; do
		eval local l_pv="\${PV${l_pv_count}}"
		eval local l_vg="\${TEST_VG_${l_pv_count}}"
		$WIPEFS_BIN -a /dev/$l_vg/* > /dev/null
		$VGREMOVE_BIN -qq -f "$l_vg"
		$PVREMOVE_BIN -qq "$l_pv"
		l_pv_count=$[l_pv_count-1]
	done

	pdebug "cleanup done"
}

function setup_pools_and_origins() {
	pdebug "entering setup_pools_and_origins()..."
	local l_pv_count=$[PV_COUNT-1]

	while [ "$l_pv_count" -ge 0 ]; do
		eval local l_vg="\${TEST_VG_${l_pv_count}}"
		pdebug "l_vg=$l_vg"
		eval local l_pool="\${TEST_POOL_${l_pv_count}}"
		pdebug "l_pool=$l_pool"
		eval local l_origin="\${TEST_ORIGIN_${l_pv_count}}"
		pdebug "l_origin=$l_origin"
		$LVCREATE_BIN -qq -L500M -T "$l_vg/$l_pool" -V 1G --name "$l_origin"
		l_pv_count=$[l_pv_count-1]
	done
	pdebug "done"
}

function setup_vgs() {
	pdebug "entering setup_vgs()..."
	local l_pv_count=$[PV_COUNT-1]

	while [ "$l_pv_count" -ge 0 ]; do
		eval local l_dev_string="\${PV${l_pv_count}}"
		pdebug "l_dev_string=$l_dev_string"
		eval local l_vg="\${TEST_VG_${l_pv_count}}"
		pdebug "l_vg=$l_vg"
		pdebug "$VGCREATE_BIN -qq $l_vg $l_dev_string"
		$VGCREATE_BIN -qq "$l_vg" "$l_dev_string"
		sleep 1
		l_pv_count=$[l_pv_count-1]
	done
	pdebug "done"
}

function setup_fs() {
	pdebug "entering setup_fs()..."
	local l_pv_count=$[PV_COUNT-1]

	while [ "$l_pv_count" -ge 0 ]; do
		eval local l_dev_path="/dev/mapper/\${TEST_VG_${l_pv_count}}-\${TEST_ORIGIN_${l_pv_count}}"
		setup_fs_$FS "$l_dev_path"
		l_pv_count=$[l_pv_count-1]
	done
	pdebug "done"
}

setup() {
	setup_vgs

	setup_pools_and_origins

	setup_fs

	mkdir $TEST_VOLUME

	mount_test_volume /dev/mapper/$TEST_VG_0-$TEST_ORIGIN_0 $TEST_VOLUME
}

usage() {
	echo "$0 -c|--command setup -f|--filesystem <ext4/btrfs/xfs>: setup test environment"
	echo "$0 -c|--command cleanup -f|--filesystem <ext4/btrfs/xfs>: cleanup test environment"
}

DEBUG=1

test $# -ge 2 || {
	usage
	exit 5
}

PV_COUNT=0

while [ "$#" -gt 0 ]; do
	case "$1" in
		--command|-c)
			COMMAND=$2;
			shift;
			;;
		--filesystem|-f)
			FS=$2
			shift;
			;;
		*)
			eval PV${PV_COUNT}="$1"
			PV_COUNT=$[PV_COUNT+1]
			;;
	esac
	shift
done

lib_checkparams || { usage; exit 100; }

register_cleanup cleanup

$COMMAND 

#case "$1" in
#	"setup")
#		register_cleanup cleanup_lvm
#		setup_lvm "$2"
#		shift 2
#		;;
#	"cleanup")
#		cleanup_lvm "$2"
#		;;
#	*)
#		eval PV$PV_COUNT=$1
#		usage;
#		exit 5;
#		;;
#esac
