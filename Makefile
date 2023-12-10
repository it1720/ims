# Název vytvořeného programu
TARGET = projekt

# Překladač
CXX = g++

# Parametry překladače
CXXFLAGS = -std=c++11 -Wall
# Adresář pro zdrojové soubory
SRC_DIR = .

# Adresář pro objektové soubory
OBJ_DIR = .

# Seznam všech zdrojových souborů
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Seznam objektových souborů generovaných z zdrojových souborů
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Hlavní cíl
all: $(TARGET)

# Cíl pro vytvoření spustitelného souboru
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Cíl pro kompilaci zdrojových souborů do objektových
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Cíl pro vyčištění projektu (smazání vytvořených souborů)
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)
