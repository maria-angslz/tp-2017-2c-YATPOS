#ifndef DATA_H_
#define DATA_H_

#include <stddef.h>

static struct {
	void *content;
	size_t size;
} data;

// Tamaño de bloque = 1 MiB
#define BLOCK_SIZE 1048576

/**
 * Carga el espacio de datos a memoria.
 * @param path Ruta al archivo binario de datos.
 * @param size Tamaño predeterminado del archivo.
 */
void data_open(const char *path, size_t size);

/**
 * Escribe el bloque de número especificado.
 * @param blockno Número de bloque.
 * @param block Datos a escribir.
 */
void data_set(int blockno, void *block);

/**
 * Obtiene un puntero al bloque de número especificado.
 * El bloque debe ser liberado con free() después de usarlo.
 * @param blockno Número de bloque.
 * @return Puntero al bloque.
 */
void *data_get(int blockno);

/**
 * Obtiene una copia del bloque especificado.
 * El bloque debe ser liberado con free() después de usarlo.
 * @param blockno Número de bloque.
 * @return Puntero a la copia del bloque.
 */
void *data_get_copy(int blockno);

/**
 * Descarga el espacio de datos de memoria.
 */
void data_close(void);

#endif /* DATA_H_ */
