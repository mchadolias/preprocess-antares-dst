ifdef ANTDSTROOT
  include $(ANTDSTROOT)/Makefile.common
else 
  include ../../Makefile.common
endif

CXXFLAGS += $(ROOTCXXFLAGS) $(ANTDSTCXXFLAGS)
LDFLAGS += $(ROOTLDFLAGS) $(ANTDSTLDFLAGS) $(MACLDFLAGS)



# directory names
SRC_DIR = $(PWD)/src
BIN_DIR = $(PWD)/bin

EXE=$(patsubst $(SRC_DIR)/%.cc, $(BIN_DIR)/%, $(wildcard $(SRC_DIR)/*.cc))

all: createdir $(EXE)

$(BIN_DIR)/%: $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	@rm -f *~ $(EXE) $(SRC_DIR)/*~
	rm -fr ${BIN_DIR}

createdir:
	@test -d "${BIN_DIR}" || (echo "Creating ${BIN_DIR}"; mkdir ${BIN_DIR})
