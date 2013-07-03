MIN_PV_COUNT=2

TEST_VOLUME=/testsuite-import-btrfs
TEST_VOLUME_X=/testsuite-import-btrfs-x

. lib/utils.sh

function cleanup() {
	umount -f $TEST_VOLUME
	umount -f $TEST_VOLUME_X

	rmdir $TEST_VOLUME
	rmdir $TEST_VOLUME_X

	local l_count=$[PV_COUNT-1]

	while [ "$l_count" -ge 0 ]; do
		eval local l_dev="\${DEV${l_count}}"
		pdebug "l_dev=$l_dev"
		$WIPEFS_BIN -a -t btrfs -f "$l_dev" > /dev/null
		l_count=$[l_count-1]
	done

	pdebug "cleanup done"
}

function setup_fs() {
	pdebug "entering setup_fs()..."
	local l_count=$[PV_COUNT-1]

	while [ "$l_count" -ge 0 ]; do
		eval local l_dev_path="\${DEV${l_count}}"
		pdebug "l_dev_path=$l_dev_path"
		setup_fs_btrfs "$l_dev_path"
		l_count=$[l_count-1]
	done
	pdebug "done"
}

setup() {
	setup_fs

	mkdir "$TEST_VOLUME"
	mkdir "$TEST_VOLUME_X"

	mount_test_volume "$DEV0" "$TEST_VOLUME"
	mount_test_volume "$DEV1" "$TEST_VOLUME_X"
}

DEBUG=1

usage() {
	echo "$0 -c|--command setup <dev0,dev1,...>: setup test environment"
	echo "$0 -c|--command cleanup <dev0,dev1,...>: cleanup test environment"
}

test $# -ge 2 || {
	usage
	exit 5
}

PV_COUNT=0
FS=btrfs

while [ "$#" -gt 0 ]; do
	case "$1" in
		--command|-c)
			COMMAND=$2;
			shift;
			;;
		*)
			eval DEV${PV_COUNT}="$1"
			PV_COUNT=$[PV_COUNT+1]
			;;
	esac
	shift
done

lib_checkparams || { usage; exit 100; }

register_cleanup cleanup

$COMMAND
