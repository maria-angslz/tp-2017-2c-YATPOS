#ifndef FILETABLE_H_
#define FILETABLE_H_

#include "yfile.h"
#include "FileSystem.h"
#include "nodelist.h"

/**
 * Inicializa la tabla de archivos.
 */
void filetable_init(void);

/**
 * Devuelve la cantidad de archivos en la tabla.
 * @return Total de archivos.
 */
int filetable_size(void);

/**
 * Devuelve el total usado por los archivos en la tabla.
 * @return Total usado.
 */
size_t filetable_totalsize(void);

/**
 * Agrega un archivo a la tabla de archivos.
 * @param file Archivo yama a agregar.
 */
void filetable_add(t_yfile *file);

/**
 * Encuentra un archivo por su ruta.
 * @param path Ruta al archivo.
 * @return Archivo yamafs.
 */
t_yfile *filetable_find(const char *path);

/**
 * Determina si existe un archivo yama en el sistema.
 * @param path Ruta al archivo.
 * @return Valor lógico indicando si el archivo existe.
 */
bool filetable_contains(const char *path);

/**
 * Mueve un archivo.
 * @param path Ruta al archivo a renombrar.
 * @param new_path Nueva ruta.
 */
void filetable_move(const char *path, const char *new_path);

/**
 * Renombra un archivo.
 * @param path Ruta al archivo a renombrar.
 * @param new_name Nuevo nombre.
 */
void filetable_rename(const char *path, const char *new_name);

/**
 * Remueve un archivo de la tabla.
 * @param Ruta al archivo.
 */
void filetable_remove(const char *path);

/**
 * Elimina todos los archivos de la tabla.
 */
void filetable_clear();

/**
 * Imprime la tabla de archivos.
 */
void filetable_print(void);

/**
 * Imprime información de un archivo.
 * @param Ruta al archivo yamafs.
 */
void filetable_info(const char *path);

/**
 * Devuelve la cantidad de archivos en un directorio.
 * @param Ruta al directorio yamafs.
 * @return Cantidad de archivos en el directorio.
 */
size_t filetable_count(const char *path);

/**
 * Imprime los archivos que pertenezcan a un determinado directorio.
 * @param path Ruta al directorio yamafs.
 */
void filetable_list(const char *path);

/**
 * Crea un archivo en el sistema de archivos yamafs copiando el
 * contenido de un archivo local.
 * @param path Ruta al archivo local que se quiere copiar.
 * @param dir Ruta de yamafs donde se guardará el archivo.
 */
void filetable_cpfrom(const char *path, const char *dir);

/**
 * Crea un archivo en el sistema de archivos local copiando el
 * contenido de un archivo yamafs.
 * @param path Ruta al archivo yamafs que se quiere copiar.
 * @param dir Ruta del sistema local donde se guardará el archivo.
 */
void filetable_cpto(const char *path, const char *dir);

/**
 * Muestra el contenido de un archivo.
 * @param path Ruta al archivo yamafs.
 */
void filetable_cat(const char *path);

/**
 * Devuelve el MD5 de un archivo YAMA.
 * @param path Ruta al archivo yamafs.
 */
char *filetable_md5(const char *path);

/**
 * Determina si el filesystem está estable.
 * @return Valor lógico indicando si hay al menos una copia
 * disponible de cada bloque.
 */
bool filetable_stable();

/**
 * Elimina determinada copia de un bloque.
 * @param file Arcchivo yama que se va a actualizar.
 * @param block Bloque del cual se eliminará la copia.
 * @param copy Copia a eliminar (0 ó 1).
 */
void filetable_rm_block(t_yfile *file, t_block* block, int copy);

/**
 * Avisa que terminó de mandarse un bloque.
 */
void filetable_sentblock(void);

/**
 * Escribe un bloque en el archivo de bloques.
 * @param node Nombre del nodo.
 * @param blockno Número de bloque.
 * @param block Contenido del bloque.
 */
void filetable_writeblock(const char *node, int blockno, void *block);

/**
 * Copia un bloque dado en otro nodo.
 * @param file Arcchivo yama que se va a actualizar.
 * @param block_free Bloque donde se copiarán los datos.
 * @param block Bloque de datos a copiar.
 * @param node Nodo donde se almacenará la copia.
 */
void filetable_cpblock(t_yfile *file, off_t block_free, t_block* block, t_node* node);

/**
 * Libera los recursos usados por la tabla de archivos.
 */
void filetable_term(void);

#endif /* FILETABLE_H_ */
