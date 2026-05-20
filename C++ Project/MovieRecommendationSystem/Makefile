# Makefile — Movie Recommendation System
#
# Alternative build file for users who prefer GNU Make directly (e.g. MinGW on Windows).
#
# Usage (MinGW / MSYS2 on Windows):
#   make
#   make clean
#
# Usage (Linux / macOS):
#   make
#   make clean
#
# Adjust MYSQL_INC and MYSQL_LIB_DIR below if MySQL is installed at a
# non-default location, or override them on the command line:
#   make MYSQL_INC="C:/custom/mysql/include" MYSQL_LIB_DIR="C:/custom/mysql/lib"

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
TARGET   := MovieRecommendationSystem

SRCS := main.cpp \
        DataStore.cpp \
        AuthManager.cpp \
        MovieManager.cpp \
        RatingManager.cpp \
        FavouritesManager.cpp \
        RecommendationEngine.cpp \
        WatchHistoryManager.cpp \
        MenuController.cpp

OBJS := $(SRCS:.cpp=.o)

# ---------------------------------------------------------------------------
# MySQL Connector/C++ (JDBC API) paths
# Adjust CONNECTOR_ROOT if the connector is installed elsewhere.
# On Linux/macOS the include and lib dirs are usually on the default search
# path, so CONNECTOR_INC and CONNECTOR_LIB_DIR can be left empty.
# ---------------------------------------------------------------------------
ifeq ($(OS),Windows_NT)
    CONNECTOR_ROOT    ?= C:/mysql-connector-c++-9.7.0-winx64/mysql-connector-c++-9.7.0-winx64
    CONNECTOR_INC     ?= $(CONNECTOR_ROOT)/include
    CONNECTOR_LIB_DIR ?= $(CONNECTOR_ROOT)/lib64/vs14
    # Link against the JDBC import library.
    CONNECTOR_LINK    := -lmysqlcppconn
else
    CONNECTOR_INC     ?=
    CONNECTOR_LIB_DIR ?=
    # On Linux/macOS the package is typically named mysqlcppconn.
    CONNECTOR_LINK    := -lmysqlcppconn
endif

# Build include / lib flags only when the paths are non-empty
MYSQL_INC_FLAG  := $(if $(CONNECTOR_INC),-I"$(CONNECTOR_INC)",)
MYSQL_LIB_FLAG  := $(if $(CONNECTOR_LIB_DIR),-L"$(CONNECTOR_LIB_DIR)",)

# ---------------------------------------------------------------------------
# Targets
# ---------------------------------------------------------------------------
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(MYSQL_LIB_FLAG) $(CONNECTOR_LINK)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(MYSQL_INC_FLAG) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	del /Q $(OBJS) $(TARGET).exe 2>nul || true
else
	rm -f $(OBJS) $(TARGET)
endif
