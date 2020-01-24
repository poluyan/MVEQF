TARGET = main
CPP = g++
CPPFLAGS = -c -ffast-math -std=c++17 -Wall -Wextra -Wpedantic -O3 -MD

SRCPATH = ./src
OBJDIR_RELEASE = obj/Release
OBJ_RELEASE = \
	$(OBJDIR_RELEASE)/main.o \
	$(OBJDIR_RELEASE)/timer.o \
	$(OBJDIR_RELEASE)/test.o \
	$(OBJDIR_RELEASE)/test1d.o \
	$(OBJDIR_RELEASE)/test2d.o \
	$(OBJDIR_RELEASE)/test3d.o \
	$(OBJDIR_RELEASE)/testNd.o \
	$(OBJDIR_RELEASE)/kde.o 

HEADERS = \
	$(SRCPATH)/trie_based.h \
	$(SRCPATH)/trie.h \
	$(SRCPATH)/timer.h \
	$(SRCPATH)/data_io.h \
	$(SRCPATH)/quantile.h \
	$(SRCPATH)/test.h \
	$(SRCPATH)/test1d.h \
	$(SRCPATH)/test2d.h \
	$(SRCPATH)/test3d.h \
	$(SRCPATH)/testNd.h \
	$(SRCPATH)/kquantile.h \
	$(SRCPATH)/kde.h \
	$(SRCPATH)/mvff.h \
	$(SRCPATH)/mveqf.h \
	$(SRCPATH)/test_kde.h 

all: release

clean: clean_release

release: before_release out_release

before_release:
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

out_release: $(OBJ_RELEASE) $(HEADERS)
	$(CPP) -o $(TARGET) $(OBJDIR_RELEASE)/*.o -pthread

$(OBJDIR_RELEASE)/main.o: $(SRCPATH)/main.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/main.cpp -o $(OBJDIR_RELEASE)/main.o
$(OBJDIR_RELEASE)/timer.o: $(SRCPATH)/timer.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/timer.cpp -o $(OBJDIR_RELEASE)/timer.o
$(OBJDIR_RELEASE)/test.o: $(SRCPATH)/test.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/test.cpp -o $(OBJDIR_RELEASE)/test.o
$(OBJDIR_RELEASE)/test1d.o: $(SRCPATH)/test.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/test1d.cpp -o $(OBJDIR_RELEASE)/test1d.o
$(OBJDIR_RELEASE)/test2d.o: $(SRCPATH)/test2d.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/test2d.cpp -o $(OBJDIR_RELEASE)/test2d.o
$(OBJDIR_RELEASE)/test3d.o: $(SRCPATH)/test3d.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/test3d.cpp -o $(OBJDIR_RELEASE)/test3d.o
$(OBJDIR_RELEASE)/testNd.o: $(SRCPATH)/testNd.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/testNd.cpp -o $(OBJDIR_RELEASE)/testNd.o
$(OBJDIR_RELEASE)/kde.o: $(SRCPATH)/kde.cpp
	$(CPP) $(CPPFLAGS) $(SRCPATH)/kde.cpp -o $(OBJDIR_RELEASE)/kde.o

clean_release:
	rm $(OBJDIR_RELEASE)/*.o
	rm $(OBJDIR_RELEASE)/*.d
	test $(TARGET) || rm $(TARGET)

-include $(OBJ_RELEASE:.o=.d)