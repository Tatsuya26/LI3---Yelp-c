CC = gcc
CFLAGS = -g  -Wall -O3 -std=c99 -D_GNU_SOURCE
PKG = `pkg-config --cflags --libs glib-2.0`
OBJ = Obj


program: $(OBJ)/main.o $(OBJ)/controlador.o $(OBJ)/catalogoR.o $(OBJ)/reviews.o $(OBJ)/catalogoU.o $(OBJ)/users.o $(OBJ)/business_network.o $(OBJ)/business.o $(OBJ)/sgr.o $(OBJ)/data.o $(OBJ)/table.o $(OBJ)/AuxStructs.o $(OBJ)/contrHandler.o $(OBJ)/interface.o 
	$(CC) $(CFLAGS) -o program $(OBJ)/main.o $(OBJ)/sgr.o $(OBJ)/controlador.o $(OBJ)/catalogoR.o $(OBJ)/reviews.o $(OBJ)/catalogoU.o $(OBJ)/users.o $(OBJ)/business_network.o $(OBJ)/business.o $(OBJ)/data.o $(OBJ)/table.o $(OBJ)/AuxStructs.o $(OBJ)/contrHandler.o $(OBJ)/interface.o  -lreadline $(PKG)

$(OBJ)/main.o: main.c
	$(CC) $(CFLAGS)  $(PKG) -c main.c -o $(OBJ)/main.o -lreadline

$(OBJ)/controlador.o: src/controller/controlador.c 
	$(CC) $(CFLAGS)  $(PKG) -c src/controller/controlador.c -o $(OBJ)/controlador.o -lreadline

$(OBJ)/sgr.o: src/Model/sgr.c Includes/Model/sgr.h
	$(CC) $(CFLAGS)  -c src/Model/sgr.c -o $(OBJ)/sgr.o $(PKG)

$(OBJ)/table.o: src/view/table.c Includes/view/table.h
	$(CC) $(CFLAGS)  -c src/view/table.c -o $(OBJ)/table.o $(PKG)

$(OBJ)/catalogoR.o: src/Model/catalogoR.c  Includes/Model/catalogoR.h
	$(CC) $(CFLAGS)  -c src/Model/catalogoR.c -o $(OBJ)/catalogoR.o $(PKG)

$(OBJ)/reviews.o: src/Model/reviews.c Includes/Model/reviews.h src/Model/data.c Includes/Model/data.h
	$(CC) $(CFLAGS) -o $(OBJ)/reviews.o -c src/Model/reviews.c  $(PKG)

$(OBJ)/catalogoU.o: src/Model/catalogoU.c Includes/Model/catalogoU.h Includes/Model/glibWarningAvoid.h
	$(CC) $(CFLAGS) -c src/Model/catalogoU.c -o $(OBJ)/catalogoU.o $(PKG)

$(OBJ)/users.o: src/Model/users.c Includes/Model/users.h Includes/Model/glibWarningAvoid.h
	$(CC) $(CFLAGS) -c src/Model/users.c -o $(OBJ)/users.o $(PKG)

$(OBJ)/business_network.o: src/Model/business_network.c Includes/Model/business_network.h
	$(CC) $(CFLAGS) $(PKG) -c src/Model/business_network.c -o $(OBJ)/business_network.o

$(OBJ)/business.o: src/Model/business.c Includes/Model/business.h
	$(CC) $(CFLAGS) -c src/Model/business.c  -o $(OBJ)/business.o $(PKG)

$(OBJ)/data.o: src/Model/data.c Includes/Model/data.h
	$(CC) $(CFLAGS) -o $(OBJ)/data.o -c src/Model/data.c    $(PKG)

$(OBJ)/AuxStructs.o: src/Model/AuxStructs.c Includes/Model/AuxStructs.h
	$(CC) $(CFLAGS) -c src/Model/AuxStructs.c -o $(OBJ)/AuxStructs.o $(PKG)

$(OBJ)/contrHandler.o :src/controller/contrHandler.c
	$(CC) $(CFLAGS)  $(PKG) -c src/controller/contrHandler.c -o $(OBJ)/contrHandler.o -lreadline

$(OBJ)/interface.o :src/view/interface.c
	$(CC) $(CFLAGS)  $(PKG) -c src/view/interface.c -o $(OBJ)/interface.o




memcheck:
	valgrind --leak-check=yes ./program

clean:
	rm -f Obj/*.o program
