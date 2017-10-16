#include "yfile.h"

#include <data.h>
#include <mlist.h>
#include <mstring.h>
#include <path.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <system.h>

// ========== Funciones públicas ==========

t_yfile *yfile_create(const char *path, t_ftype type) {
	t_yfile *file = malloc(sizeof(t_yfile));
	file->path = mstring_duplicate(path);
	file->size = 0;
	file->type = type;
	file->blocks = mlist_create();
	return file;
}

void yfile_addblock(t_yfile *file, t_block *block) {
	block->index = mlist_length(file->blocks);
	mlist_append(file->blocks, block);
	file->size += block->size;
}

void yfile_print(t_yfile *file) {
	printf("==== Archivo yamafs ====\n");
	if(file == NULL) {
		printf("(Archivo inexistente)\n");
		return;
	}
	printf("Ruta: %s\n", file->path);
	printf("Tipo: %s\n", file->type == FTYPE_TXT ? "TEXTO" : "BINARIO");
	printf("Tamaño: %d\n", file->size);
	printf("Bloques: %d\n", mlist_length(file->blocks));
	int blockno = 0;
	void routine(t_block *block) {
		printf("Bloque %d:\n", blockno++);
		printf(" Tamaño: %d\n", block->size);
		printf(" Copia 0: [%s, %d]\n", block->copies[0].node, block->copies[0].blockno);
		printf(" Copia 1: [%s, %d]\n", block->copies[1].node, block->copies[1].blockno);
	}
	mlist_traverse(file->blocks, routine);
}

t_serial *yfile_pack(t_yfile *file) {
	t_serial *serial = serial_create(NULL, 0);
	serial_add(serial, "siii", file->path, file->size, file->type, mlist_length(file->blocks));

	void routine(t_block *block) {
		serial_add(serial, "iiisis", block->index, block->size,
				block->copies[0].blockno, block->copies[0].node,
				block->copies[1].blockno, block->copies[1].node);
	}
	mlist_traverse(file->blocks, routine);
	return serial;
}

t_yfile *yfile_unpack(t_serial *serial) {
	t_yfile *file = yfile_create(NULL, 0);

	int numblocks;
	serial_remove(serial, "siii", &file->path, &file->size, &file->type, &numblocks);

	while(numblocks--) {
		t_block *block = malloc(sizeof(t_block));
		serial_remove(serial, "iiisis", &block->index, &block->size,
				&block->copies[0].blockno, &block->copies[0].node,
				&block->copies[1].blockno, &block->copies[1].node);
		mlist_append(file->blocks, block);
	}

	return file;
}

void yfile_destroy(t_yfile *file) {
	free(file->path);
	mlist_destroy(file->blocks, free);
	free(file);
}

// ========== Funciones privadas ==========
