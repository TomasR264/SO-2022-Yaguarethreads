#include "../include/protocolo.h"


static void* serializar_NO_OP(uint32_t parametro1) {
   void* stream = malloc(sizeof(op_code_instrucciones) + sizeof(uint32_t));

    op_code_instrucciones cop = NO_OP;
    memcpy(stream, &cop, sizeof(op_code_instrucciones));
    memcpy(stream+sizeof(op_code_instrucciones), &parametro1, sizeof(uint32_t));

    printf("El cop en serializar NO_OP es: %d\n",cop);
    printf("El parametro 1 es: %d\n",parametro1);
    printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


static void deserializar_NO_OP_2(void* stream, uint32_t* parametro1) {
	size_t tam_param;

    memcpy(parametro1, stream ,sizeof(uint32_t));

}

bool send_NO_OP(int fd, uint32_t parametro1) {
	printf("Entre en send_NO_OP \n");
   size_t size = sizeof(op_code_instrucciones) + sizeof(uint32_t);

    void* stream = serializar_NO_OP(parametro1);


    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    free(stream);
    return true;
}


bool recv_NO_OP_2(int fd, uint32_t* parametro1) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_NO_OP_2(stream, parametro1);

    free(stream);
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void* serializar_IO(uint32_t parametro1) {
   void* stream = malloc(sizeof(op_code_instrucciones) + sizeof(uint32_t));

    op_code_instrucciones cop = IO;
    memcpy(stream, &cop, sizeof(op_code_instrucciones));
    memcpy(stream+sizeof(op_code_instrucciones), &parametro1, sizeof(uint32_t));

    printf("El cop en serializar IO es: %d\n",cop);
    printf("El parametro 1 es: %d\n",parametro1);
    printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


static void deserializar_IO(void* stream, uint32_t* parametro1) {
	size_t tam_param;

    memcpy(parametro1, stream ,sizeof(uint32_t));

}

bool send_IO(int fd, uint32_t parametro1) {
	printf("Entre en send_IO \n");
   size_t size = sizeof(op_code_instrucciones) + sizeof(uint32_t);

    void* stream = serializar_IO(parametro1);


    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    free(stream);
    return true;
}


bool recv_IO(int fd, uint32_t* parametro1) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_IO(stream, parametro1);

    free(stream);
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void* serializar_READ(uint32_t parametro1) {
   void* stream = malloc(sizeof(op_code_instrucciones) + sizeof(uint32_t));

    op_code_instrucciones cop = READ;
    memcpy(stream, &cop, sizeof(op_code_instrucciones));
    memcpy(stream+sizeof(op_code_instrucciones), &parametro1, sizeof(uint32_t));

    printf("El cop en serializar READ es: %d\n",cop);
    printf("El parametro 1 es: %d\n",parametro1);
    //printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


static void deserializar_READ(void* stream, uint32_t* parametro1) {
	size_t tam_param;

    memcpy(parametro1, stream ,sizeof(uint32_t));

}

bool send_READ(int fd, uint32_t parametro1) {
	printf("Entre en send_READ \n");
   size_t size = sizeof(op_code_instrucciones) + sizeof(uint32_t);

    void* stream = serializar_READ(parametro1);


    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    free(stream);
    return true;
}


bool recv_READ(int fd, uint32_t* parametro1) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_READ(stream, parametro1);

    free(stream);
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

static void* serializar_WRITE(uint32_t parametro1, uint32_t parametro2) {
    void* stream = malloc(sizeof(op_code_instrucciones) + sizeof(uint32_t) * 2);

    op_code_instrucciones cop = WRITE;
    memcpy(stream, &cop, sizeof(op_code_instrucciones));
    memcpy(stream+sizeof(op_code_instrucciones), &parametro1, sizeof(uint32_t));
    memcpy(stream+sizeof(op_code_instrucciones)+sizeof(uint32_t), &parametro2, sizeof(uint32_t));

    printf("El cop en serializar WRITE es: %d\n",cop);
    printf("El parametro 1 es: %d\n",parametro1);
    printf("El parametro 2 es: %d\n",parametro2);
   // printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));

    return stream;
}

static void deserializar_WRITE(void* stream, uint32_t* parametro1, uint32_t* parametro2) {
    memcpy(parametro1, stream, sizeof(uint32_t));
    memcpy(parametro2, stream+sizeof(uint32_t), sizeof(uint32_t));
}

bool send_WRITE(int fd, uint32_t parametro1, uint32_t parametro2) {
	printf("Entre en send_WRITE \n");
    size_t size = sizeof(op_code_instrucciones) + sizeof(uint32_t) * 2;
    void* stream = serializar_WRITE(parametro1, parametro2);
    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }
    free(stream);
    return true;
}

bool recv_WRITE(int fd, uint32_t* parametro1, uint32_t* parametro2) {
    size_t size = sizeof(uint32_t) * 2;
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_WRITE(stream, parametro1, parametro2);

    free(stream);
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void* serializar_COPY(uint32_t parametro1, uint32_t parametro2) {
    void* stream = malloc(sizeof(op_code_instrucciones) + sizeof(uint32_t) * 2);

    op_code_instrucciones cop = COPY;
    memcpy(stream, &cop, sizeof(op_code_instrucciones));
    memcpy(stream+sizeof(op_code_instrucciones), &parametro1, sizeof(uint32_t));
    memcpy(stream+sizeof(op_code_instrucciones)+sizeof(uint32_t), &parametro2, sizeof(uint32_t));

    printf("El cop en serializar WRITE es: %d\n",cop);
    printf("El parametro 1 es: %d\n",parametro1);
    printf("El parametro 2 es: %d\n",parametro2);
  //  printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));

    return stream;
}

static void deserializar_COPY(void* stream, uint32_t* parametro1, uint32_t* parametro2) {
    memcpy(parametro1, stream, sizeof(uint32_t));
    memcpy(parametro2, stream+sizeof(uint32_t), sizeof(uint32_t));
}

bool send_COPY(int fd, uint32_t parametro1, uint32_t parametro2) {
	printf("Entre en send_COPY \n");
    size_t size = sizeof(op_code_instrucciones) + sizeof(uint32_t) * 2;
    void* stream = serializar_COPY(parametro1, parametro2);
    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }
    free(stream);
    return true;
}

bool recv_COPY(int fd, uint32_t* parametro1, uint32_t* parametro2) {
    size_t size = sizeof(uint32_t) * 2;
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_COPY(stream, parametro1, parametro2);

    free(stream);
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void* serializar_EXIT() {
   void* stream = malloc(sizeof(op_code_instrucciones));

    op_code_instrucciones cop = EXIT;
    memcpy(stream, &cop, sizeof(op_code_instrucciones));


    printf("El cop en serializar EXIT es: %d\n",cop);

  //  printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


static void deserializar_EXIT(void* stream) {
	size_t tam_param;

    // memcpy(parametro1, stream ,sizeof(uint32_t));

}

bool send_EXIT(int fd) {
	printf("Entre en send_EXIT \n");
   size_t size = sizeof(op_code_instrucciones);

    void* stream = serializar_EXIT();


    if (send(fd, stream, size, 0) != size) { //ACA EN EL SEND PUSE EL EXIT, ANTES HABIA UN STREAM
      //  free(stream);
        return false;
    }

   // free(stream);
    return true;
}


bool recv_EXIT(int fd) {
    size_t size = sizeof(op_code_instrucciones);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

   // deserializar_EXIT(stream);

    free(stream);
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////


static void* serializar_TAM(uint32_t parametro1) {
   void* stream = malloc(sizeof(uint32_t));
   // op_code_instrucciones cop = READ;
    //memcpy(stream, &cop, sizeof(op_code_instrucciones));
    memcpy(stream, &parametro1, sizeof(uint32_t));

   // printf("El cop en serializar READ es: %d\n",cop);
    printf("El tam a enviar es: %d\n",parametro1);
   // printf("El tam del stream cuando lo serializamos es %d\n", sizeof(stream));
    return stream;
}


void deserializar_TAM(void* stream, uint32_t* parametro1) {

    memcpy(parametro1, stream ,sizeof(uint32_t));
//  printf("El tam en deserializar tam es: %d \n", parametro1);

}

bool send_TAM(int fd, uint32_t parametro1) {
	printf("Entre en send_TAM \n");
   size_t size = sizeof(uint32_t);

    void* stream = serializar_TAM(parametro1);


    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    free(stream);
    return true;
}


bool recv_TAM(int fd, uint32_t* parametro1) {
    size_t size = sizeof(uint32_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_TAM(stream, parametro1);

    free(stream);
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//\

//HAY UN INVALID WRITE, PERO NO SE DONDE ES QUE ESCRIBE DE MAS
void* serializar_PCB(pcb_t* pcb) {
	printf("El size del pcb en serializar es: %d\n",sizeof(pcb_t));
	printf("El size del tlist en serializar es: %d\n",sizeof(t_list));
    void* stream = malloc(sizeof(pcb_t));
    if (stream == NULL) return NULL;

    memcpy(stream, &pcb->PID, sizeof(uint32_t));
    memcpy(stream+sizeof(uint32_t), &pcb->tamanio, sizeof(uint32_t));
    memcpy(stream+sizeof(uint32_t)+sizeof(uint32_t), &pcb->instrucciones, sizeof(t_list));
    memcpy(stream+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(t_list), &pcb->PC, sizeof(uint32_t));
  //  memcpy(stream+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(t_list)+sizeof(uint32_t), &pcb->tabla_paginas, sizeof(t_list));
///    memcpy(stream+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(t_list)+sizeof(uint32_t)+sizeof(t_list), &pcb->estimacionRafaga, sizeof(uint32_t));
 ///   memcpy(stream+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(t_list)+sizeof(uint32_t)+sizeof(t_list)+sizeof(uint32_t), &pcb->alpha, sizeof(uint32_t));//este HAY QUE CAMBIARLO NO ES UINT

    return stream;
}

void deserializar_PCB(void* stream,pcb_t* pcb) {
	pcb = malloc(sizeof(pcb_t));
    if (pcb == NULL){
    	printf("error al deserializar el pcb \n");
    }
    memcpy(&pcb->PID, stream, sizeof(uint32_t));
    memcpy(&pcb->tamanio, stream+sizeof(uint32_t), sizeof(uint32_t));
    memcpy(&pcb->instrucciones, stream+sizeof(uint32_t)+sizeof(uint32_t), sizeof(t_list));
    memcpy(&pcb->PC, stream+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(t_list), sizeof(uint32_t));
    //memcpy(&pcb->tabla_paginas, stream+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(t_list)+sizeof(uint32_t), sizeof(t_list));
    //memcpy(&pcb->estimacionRafaga, stream+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(t_list)+sizeof(uint32_t)+sizeof(t_list), sizeof(uint32_t));
  //  memcpy(&pcb->alpha, stream+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(t_list)+sizeof(uint32_t)+sizeof(t_list)+sizeof(uint32_t), sizeof(uint32_t));

}

//estas 2, send y rcv no se si van
bool send_PCB(int fd, pcb_t* parametro1) {
	printf("Entre en send_PCB \n");
   size_t size = sizeof(pcb_t);

    void* stream = serializar_PCB(parametro1);


    if (send(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    free(stream);
    return true;
}

bool recv_PCB(int fd, pcb_t** parametro1) {
	printf("Entre en send_PCB \n");
    size_t size = sizeof(pcb_t);
    void* stream = malloc(size);

    if (recv(fd, stream, size, 0) != size) {
        free(stream);
        return false;
    }

    deserializar_PCB(stream,parametro1);

    free(stream);
    return true;
}

