#------------------------------------------------------------
# please set each item

# source files
SRCS_t   = main_test.cpp FNV_Hash.cpp
SRCS_bm  = main_bench.cpp FNV_Hash.cpp
SRCS_sp  = main_sProc.cpp
HEADS    = CHashT.hpp IpCHashT.hpp
HEADS_t  = test_CHashT.hpp test_IpCHashT.hpp
HEADS_bm = bench.hpp

# name of generating file
TARGET_t  = exe_test  # test
TARGET_bm = exe_bench # benchmark
TARGET_sp = exe_sProc # Statistical processing

# remove files
RMs = *.stackdump __pycache__ tmpDir

# compilation options
CFLAGS += -L./sstd/lib -I./sstd/include -lsstd # sstd
CFLAGS += -L./googletest-master/build/lib -I./googletest-master/googletest/include -lgtest -pthread # google test
CFLAGS += -I./sparsehash-master/src # sparsehash
CFLAGS += -std=c++11 # CFLAGS += -std=gnu++0x
CFLAGS += -Wall
#CFLAGS += -Wextra
CFLAGS += -O3

CFLAGS_t  += -DEBUG
CFLAGS_bm += -DNDEBUG
CFLAGS_sp += -DNDEBUG

#------------------------------------------------------------

BACKUP_DIR   = ./backup
ALL_FILES    = $(wildcard ./*)
TMP_DIRS     = $(wildcard ./tmp*)
LIBS_DIRS    = ./sstd ./googletest-master ./sparsehash-master
BACKUP_FILES = $(filter-out $(TMP_DIRS) $(LIBS_DIRS) ./$(TARGET_t) ./$(TARGET_bm) $(BACKUP_DIR), $(ALL_FILES))
TIME_STAMP   = `date +%Y_%m%d_%H%M`


# when you need to check the change of files in lib, you need to change file name to a not-existing name like "FORCE_XXX".
#LIB_SSTD       = FORCE_SSTD
#LIB_GOOGLETEST = FORCE_GOOGLETEST
LIB_SSTD       = ./sstd/lib/libsstd.a
LIB_GOOGLETEST = ./googletest-master/build/lib/libgtest.a
LIB_flat       = ./flat_hash_map-master/flat_hash_map.hpp
LIB_sparsehash = ./sparsehash-master/src/sparsehash/internal/sparseconfig.h


# generate exe file
TARGET_all = FORCE_MAKEALL
$(TARGET_all): $(TARGET_sp) $(TARGET_bm) $(TARGET_t)
	@echo "make all"
$(TARGET_sp): $(SRCS_sp)
	@echo ""
	@echo "------------------------------------------------------------"
	@echo "SRCS_st: \n$(SRCS_sp)\n"
	@echo "CFLAGS: \n$(CFLAGS)"
	@echo "------------------------------------------------------------"
	$(CXX) -o $(TARGET_sp) $(SRCS_sp) $(CFLAGS) $(CFLAGS_sp)
	@echo ""
$(TARGET_bm): $(LIB_SSTD) $(LIB_GOOGLETEST) $(LIB_flat) $(LIB_sparsehash) $(SRCS_bm) $(HEADS) $(HEADS_bm)
	@echo ""
	@echo "------------------------------------------------------------"
	@echo "SRCS_bm: \n$(SRCS_bm)\n"
	@echo "CFLAGS: \n$(CFLAGS)"
	@echo "------------------------------------------------------------"
	$(CXX) -o $(TARGET_bm) $(SRCS_bm) $(CFLAGS) $(CFLAGS_bm)
	@echo ""
$(TARGET_t): $(LIB_SSTD) $(LIB_GOOGLETEST) $(LIB_flat) $(SRCS_t) $(HEADS) $(HEADS_t)
	@echo ""
	@echo "------------------------------------------------------------"
	@echo "SRCS_t: \n$(SRCS_t)\n"
	@echo "CFLAGS: \n$(CFLAGS)"
	@echo "------------------------------------------------------------"
	$(CXX) -o $(TARGET_t) $(SRCS_t) $(CFLAGS) $(CFLAGS_t)
	@echo ""


$(LIB_SSTD):
	@echo ""
	@unzip -n SubStandardLibrary-SSTD--master.zip
	@mv SubStandardLibrary-SSTD--master/sstd ./
	@rm -rf SubStandardLibrary-SSTD--master/
	@(cd ./sstd; make -j)

$(LIB_flat):
	@echo ""
	@unzip -n flat_hash_map-master.zip

$(LIB_GOOGLETEST):
	@echo ""
	@unzip -n googletest-master.zip
	@(cd ./googletest-master; mkdir -p build; cd build; cmake ..; make)

$(LIB_sparsehash):
	@echo ""
	@unzip -n sparsehash-master.zip
	@(cd ./sparsehash-master; ./configure; make -j)


.PHONY: all
all:
	@(make clean)
	@(make)


.PHONY: clean
clean:
	-rm -rf $(TARGET_all)
	-rm -rf flat_hash_map-master
	-rm -rf googletest-master
	-rm -rf sparsehash-master
	-rm -rf sstd
	-rm -rf $(RMs)
#	$(if $(patch_txt_exists) ,$(rm *.stackdump),)
#	-rm -f $(OBJS) $(DEPS) $(TARGET)


.PHONY: zip
zip:
	-rm -rf $(RMs)
	@mkdir -p $(BACKUP_DIR)
	zip -r $(BACKUP_DIR)/${TIME_STAMP}$(m).zip $(BACKUP_FILES)
.PHONY: backup
backup:
	@(make zip)
	@(make)
# when you need comments for backup, just type
# $ make backup m=_comment_will_be_inserted_after_the_date

.PHONY: updateLib
updateLib:
	wget https://github.com/admiswalker/SubStandardLibrary-SSTD-/archive/master.zip -O SubStandardLibrary-SSTD--master.zip
	-rm -rf sstd # remove previous versions of sstd

.PHONY: steps
steps: $(SRCS_t) $(SRCS_bm) $(HEADS) $(HEADS_t) $(HEADS_bm)
	@echo "$^" | xargs wc -l
	@echo ""
	@(cd ./sstd; make steps)

