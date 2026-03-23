# =========================================
# Tarih: 23.02.2026
# CROSS-PLATFORM MAKEFILE (C & C++17)
# Geliştirici: Gemini 3.1 Pro & Abdulkadir 
# =========================================

# --- 1. DERLEYİCİ SEÇİMİ ---
CC := gcc
CXX := g++

# --- 2. DİZİN YAPILANDIRMASI ---
SRC_DIR := src
INC_DIR := inc
OBJ_DIR := lib
BIN_DIR := bin
APP_NAME := app

# --- 3. DERLEME BAYRAKLARI (FLAGS) ---
# -Wall -Wextra: Tüm potansiyel hata uyarılarını açar (Clean Code için şart).
# -pedantic: C/C++ standartlarına sıkı sıkıya uyulmasını zorunlu kılar.
# -g: Hata ayıklama (Debug) sembollerini koda gömer (SegFault bulmak için kritik!).
# -I$(INC_DIR): Derleyiciye başlık dosyalarını (header) nerede arayacağını söyler.
CFLAGS := -I$(INC_DIR) -Wall -Wextra -pedantic -std=c11 -g
CXXFLAGS := -I$(INC_DIR) -Wall -Wextra -pedantic -std=c++23 -O3 -g
# Windows ve C++23 <print> kütüphanesi için zorunlu Linker bayrağı
LDFLAGS := -lstdc++exp

# --- 4. DOSYA TESPİTİ ---
# src klasöründeki tüm .c ve .cpp dosyalarını otomatik bul.
C_SRCS := $(wildcard $(SRC_DIR)/*.c)
CXX_SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Bulunan kaynak dosyaların isimlerini .o (obje) formatına çevirip obj klasörüne yönlendir.
C_OBJS := $(C_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
CXX_OBJS := $(CXX_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJS := $(C_OBJS) $(CXX_OBJS)

# --- 5. İŞLETİM SİSTEMİ (OS) KONTROLÜ VE KOMUTLAR ---
ifeq ($(OS),Windows_NT)
	TARGET := $(BIN_DIR)/$(APP_NAME).exe
	# Windows'ta klasörleri ve içindekileri sessizce sil
	CLEAN_CMD := if exist $(OBJ_DIR) rd /s /q $(OBJ_DIR) & if exist $(BIN_DIR) rd /s /q $(BIN_DIR)
	# Windows'ta klasör yoksa oluştur
	MKDIR_CMD := if not exist $(OBJ_DIR) mkdir $(OBJ_DIR) & if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	SCREEN_CLEAR := cls
	SLEEP_CMD := timeout /t 1 /nobreak > NUL
else
	TARGET := $(BIN_DIR)/$(APP_NAME)
	# Linux/Mac'te klasörleri ve içindekileri zorla sil
	CLEAN_CMD := rm -rf $(OBJ_DIR) $(BIN_DIR)
	# Linux/Mac'te klasör yoksa oluştur
	MKDIR_CMD := mkdir -p $(OBJ_DIR) $(BIN_DIR)
	SCREEN_CLEAR := clear
	SLEEP_CMD := sleep 1
endif

# --- 6. DERLEME KURALLARI ---

# 'make' yazıldığında çalışacak varsayılan kural. Önce klasörler, sonra derleme.
compile: prepare $(TARGET)

# Klasörlerin var olduğundan emin ol.
prepare:
	@$(MKDIR_CMD)

# Linkleme (Bağlama) İşlemi: Tüm .o dosyalarını al ve çalıştırılabilir programı (app) üret.
$(TARGET): $(OBJS)
	@echo --- [LINK] Program baglaniyor: $@ ---
	@$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	@echo --- [BASARILI] Derleme tamamlandi! ---

# C++ dosyalarını derleme kuralı ($<: kaynak dosya, $@: hedef obje dosyası)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo [C++] Derleniyor: $<
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# C dosyalarını derleme kuralı
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo [C]   Derleniyor: $<
	@$(CC) $(CFLAGS) -c $< -o $@


run: compile
	@echo --- [RUN] Proje calistiriliyor ---
	@$(TARGET) $(ARGS)
	@echo --- [BASARILI] Yurutme tamamlandi ---

# --- 7. TEMİZLİK ---
# 'make clean' yazıldığında derlenmiş dosyaları siler, projeyi sıfırlar.
clean:
	@echo --- [CLEAN] Proje temizleniyor ---
	@$(CLEAN_CMD)
	@echo --- [BASARILI] Temizlik bitti ---
	@$(SLEEP_CMD) 
	@$(SCREEN_CLEAR)


# .PHONY: Bu isimlerde dosyalar olsa bile komut olarak algılanmalarını sağlar.
.PHONY: compile prepare run clean