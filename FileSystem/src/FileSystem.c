#include "FileSystem.h"
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include "console.h"
#include "server.h"
#include "estructuras.h"
#include <commons/collections/list.h>
#include <commons/config.h>
#include <system.h>
#include <mstring.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdbool.h>
#include <mlist.h>
#include <string.h>
#include <file.h>
#include <path.h>
#include <thread.h>
#include "dirtree.h"
#include "nodelist.h"
#include "yfile.h"
#include <path.h>
#include <bitmap.h>
#include <protocol.h>
#include <config.h>
#include "filetable.h"
#include <serial.h>

int indexDirectorio = 0;

static void init(void);
static void clear_previous_state(void);
static void term(void);

// ========== Funciones públicas ==========

int main(int argc, char *argv[]) {
//	if(argc == 2 && mstring_equal(argv[1], "--clean")) {
		clear_previous_state();
//	}

	init();

	server();
	console();

	term();
	return EXIT_SUCCESS;
}

// ========== Funciones privadas ==========

static void init() {
	process_init();
	dirtree_init();
	filetable_init();
	nodelist_init();
	fs.formatted = nodelist_size() > 0;
}

static void clear_previous_state() {
	path_remove("metadata");
	fs.formatted = false;
}

static void term() {
	thread_killall();
	nodelist_term();
	dirtree_term();
	process_term();
}




char *config_file(char* NombreEstructura,char* extension) {
	return mstring_create("%s/metadata/%s.%s", system_userdir(), NombreEstructura,extension);
}

void inicializarEstructurasFilesystem(){
	inicializarTablaDirectorio();
	inicializarTablaArchivos();
	inicializarTablaNodos();
	inicializarBitmap();
}

void guardarEstructuras(){
	config_save(archivoDirectorio);
}

void crearDirectorio(int padre,char* nombreDirectorio){
	//creo la raiz
	char* indice = malloc(sizeof(char));
	tablaDirectorio[indexDirectorio].index = indexDirectorio;
	tablaDirectorio[indexDirectorio].padre = padre;
	strcpy(tablaDirectorio[indexDirectorio].nombre,nombreDirectorio);
	char* cadena = malloc((sizeof(char)*255)+2*sizeof(char));
	sprintf(indice,"%d",indexDirectorio);
	sprintf(cadena,"[%d,%s,%d]",indexDirectorio,tablaDirectorio[indexDirectorio].nombre,tablaDirectorio[indexDirectorio].padre);
	config_set_value(archivoDirectorio,indice,cadena);
	indexDirectorio ++;
	free(cadena);
	free(indice);
}

void inicializarTablaDirectorio(){
	//al iniciar debe verificar si existe un estado anterior
	char* rutaArchivo = mstring_create("%s/metadata/directorios.dat", system_userdir());
	archivoDirectorio = config_create(rutaArchivo);
	if(archivoDirectorio == NULL){
		estadoAnteriorexistente = false;
		fopen(rutaArchivo,"w+");
		archivoDirectorio = config_create(rutaArchivo);
		crearDirectorio((-1),"root");
		config_save(archivoDirectorio); //momentaneamente para probar que funciona
	}
	else{
		//las debe levantar y esperar conexion de datanodes
		estadoAnteriorexistente = true;
		char* indice = malloc(sizeof(char));
		for(indexDirectorio=0;indexDirectorio<config_keys_amount(archivoDirectorio);indexDirectorio++){
			sprintf(indice,"%d",indexDirectorio);
			char** arrayAuxiliar = config_get_array_value(archivoDirectorio,indice);
			tablaDirectorio[indexDirectorio].index = atoi(arrayAuxiliar[0]);
			strcpy(tablaDirectorio[indexDirectorio].nombre,arrayAuxiliar[1]);
			tablaDirectorio[indexDirectorio].padre = atoi(arrayAuxiliar[2]);
		}
		free(indice);
	}
	free(rutaArchivo);
}


void inicializarTablaArchivos(){
	DIR* directory;
	t_config* configArchivos;
	struct dirent *entradaArchivo;
	char* rutaArchivo = malloc(22*sizeof(char*)+sizeof(int));
	directory = opendir("/home/utnso/yatpos/metadata/archivos");
	listaArchivosYamaFS = list_create();
	int i;
	char* indice= malloc(18*sizeof(char));
	char** arrayAuxiliar;
	while((entradaArchivo = readdir(directory)) != NULL && strcmp(entradaArchivo->d_name, ".")) {
		t_Archivo* tablaArchivo = malloc(sizeof(t_Archivo));
	  /* Cualquier entrada en este ciclo se trata de un archivo existente en el directorio */
		sprintf(rutaArchivo,"/home/utnso/yatpos/metadata/archivos/%s/ejemplo.csv",entradaArchivo->d_name);
		configArchivos = config_create(rutaArchivo);
		tablaArchivo->tamanio = config_get_long_value(configArchivos,"TAMANIO");
		tablaArchivo->tipo = config_get_string_value(configArchivos,"TIPO");
		tablaArchivo->bloques = list_create();
		for(i=0;i<((config_keys_amount(configArchivos)-2)/3);i++){

			bloque* bloques= malloc(sizeof(bloque));
			sprintf(indice,"BLOQUE%dCOPIA0",i);
			arrayAuxiliar = config_get_array_value(configArchivos,indice);
			bloques->copia0[0] = arrayAuxiliar[0];
			bloques->copia0[1] = arrayAuxiliar[1];
			sprintf(indice,"BLOQUE%dCOPIA1",i);
			arrayAuxiliar = config_get_array_value(configArchivos,indice);
			bloques->copia1[0] = arrayAuxiliar[0];
			bloques->copia1[1] = arrayAuxiliar[1];
			sprintf(indice,"BLOQUE%dBYTES",i);
			bloques->bytes = config_get_long_value(configArchivos,indice);
			list_add_in_index(tablaArchivo->bloques,i,bloques);

		}
		list_add(listaArchivosYamaFS,tablaArchivo);


	}
	free(indice);
	closedir(directory);
	free(rutaArchivo);
}

void inicializarTablaNodos(){
	//al iniciar debe verificar si existe un estado anterior
	char* rutaArchivo = config_file("nodos","bin");
	archivoNodos = config_create(rutaArchivo);
	tablaNodos.nodos = list_create();
	tablaNodos.tamanio = 0;
	tablaNodos.libre = 0;
	if(archivoNodos == NULL){
		//en caso que no exista, se crea uno.
		fopen(rutaArchivo,"w+");
		archivoNodos = config_create(rutaArchivo);
	}
	else{
		//las debe levantar y esperar conexion de datanodes
		tablaNodos.tamanio = config_get_int_value(archivoNodos,"TAMANIO");
		tablaNodos.libre = config_get_int_value(archivoNodos,"LIBRE");
		char** arrayAuxiliar = config_get_array_value(archivoNodos,"NODOS");
		int i;
		char* key = malloc(8*sizeof(char*));
		for(i=0;i<(sizeof(arrayAuxiliar)-1);i++){
			Nodo* nodo = malloc(sizeof(Nodo));
			nodo->nombreNodo = arrayAuxiliar[i];
			sprintf(key,"%sTotal",arrayAuxiliar[i]);
			nodo->total = config_get_int_value(archivoNodos,key);
			sprintf(key,"%sLibre",arrayAuxiliar[i]);
			nodo->libre = config_get_int_value(archivoNodos,key);
			list_add(tablaNodos.nodos,nodo);
		}
		free(key);
		free(arrayAuxiliar);
	}

	free(rutaArchivo);
}

void inicializarBitmap(){
	DIR* directory;
	struct dirent *entradaArchivo;
	listaBitmaps = list_create();
	t_config* configBitmapNodo;
	char* rutaArchivo = malloc(22*sizeof(char*)+sizeof(int));
	directory = opendir("/home/utnso/yatpos/metadata/bitmaps");
	while((entradaArchivo = readdir(directory)) != NULL && strcmp(entradaArchivo->d_name, ".")) {
		sprintf(rutaArchivo,"/home/utnso/yatpos/metadata/bitmaps/%s",entradaArchivo->d_name);
		configBitmapNodo = config_create(rutaArchivo);
		char** bitmap = config_get_array_value(configBitmapNodo,"BITMAP");
		list_add(listaBitmaps,bitmap);
	}
}
