#
# Makefile.am for snapper/testsuite-import
#


AM_CXXFLAGS = -std=gnu++0x -DBOOST_TEST_DYN_LINK

INCLUDES = -I$(top_srcdir)

LDADD = ../snapper/libsnapper.la -lboost_unit_test_framework

#noinst_PROGRAMS = tsmain
noinst_PROGRAMS = testsuitemain

#tsmain_SOURCES = tsmain.cc lvmimportmetadata-fixtures.cc helpers.cc general-fixtures.cc
testsuitemain_SOURCES = testsuitemain.cc lvm-master-testsuite.cc lvmimportmetadata-test.cc \
			lvmimportmetadata-fixtures.cc lvm-test.cc lvm-fixtures.cc helpers.cc \
			general-fixtures.cc snapshot-test.cc snapshot-fixtures.cc \
			btrfs-testsuite.cc btrfsimportmetadata-test.cc btrfsimportmetadata-fixtures.cc

