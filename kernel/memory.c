#include "memory.h"

#pragma pack(1)
struct block_s {
	size_t size;
	block_t* next;
	byte free_block;
};

#define BLOCK_SIZE sizeof(block_t)

/**Basicamente, este array é toda nossa área de alocação dinâmica de memória.*/
byte s_heap[HEAP_SIZE];
/**Contém o ponteiro para o primeiro bloco criado.*/
block_t* s_blocks = NULL;

//------------------------------------------------------------------------------
// Funções auxiliares para serem utilizados dentro do MALLOC e FREE.
//------------------------------------------------------------------------------
block_t* _find_free_block( size_t a_size );
void _split_free_block( block_t* a_block, size_t a_resize );
void _merge_free_blocks();
//------------------------------------------------------------------------------

void* malloc( size_t a_size ) {
	if( a_size == 0 ) {
		return NULL;
	}
	block_t* l_block = s_blocks;
	// No caso de a lista de blocos estar vazia, cria um bloco novo, inicializa ele e retorna.
	if( l_block == NULL ) {
		s_blocks = (block_t*)s_heap;
		l_block = s_blocks;
	} else {
		// Tenta reutilizar um bloco já existente que tenha sido liberado anteriormente.
		block_t* l_free = _find_free_block( a_size );
		if( l_free ) {
			// Verifica se esse bloco livre pode ser dividido
			if( ( a_size + BLOCK_SIZE ) < l_free->size ) {
				// Se puder, divide o bloco, criando um bloco posterior a l_free com o espaço restante.
				_split_free_block( l_free, a_size );
			}
			// Por fim, marca o bloco como ocupado e retorna ele.
			l_block = l_free;
			l_block->free_block = 0;
			return l_block + 1;
		} else {
			// Não existe um bloco vago disponivel, logo, vai ter que criar um.
			size_t l_offset = BLOCK_SIZE + l_block->size;
			while( l_block->next != NULL ) {
				l_offset += BLOCK_SIZE + l_block->next->size;
				l_block = l_block->next;
			}
			// Verifica se ainda existe espaço na pilha
			if( ( BLOCK_SIZE + l_offset + a_size ) >= HEAP_SIZE ) {
				// Se não tiver, retorna nulo
				return NULL;
			}
			// Existe espaço na pilha, então pega o endereço do bloco e retorna o endereço da parte de dados.
			block_t* l_previous = l_block;
			l_block = (block_t*)(&s_heap[l_offset]);
			l_previous->next = l_block;
		}
	}
	
	l_block->size = a_size;
	l_block->free_block = 0;
	l_block->next = NULL;
	
	return l_block + 1;
}

void free( void* a_pointer ) {
	// Não precisa fazer nada se o ponteiro for nulo.
	if( a_pointer == NULL ) {
		return;
	}
	// Recupera o bloco do ponteiro dado
	block_t* l_block = ((block_t*) a_pointer ) - 1;
	// TODO: fazer uma exceção, caso o valor de free seja LIVRE nesse ponto.
	// assert(block_ptr->free == 0);
	l_block->free_block = 1;
	// Agora, tenta fundir quaisquer blocos que estejam livres e adjacentes.
	_merge_free_blocks();
}

/**
 * Procura por um bloco que tenha sido liberado anteriormente e que seja capaz 
 * de suportar o tamanho requisitado.
 * @param a_previous retorna o bloco anterior ao encontrado.
 * @
 */
block_t* _find_free_block( size_t a_size ) {
	block_t* l_current = s_blocks;
	// Procura pelo primeiro bloco que estiver disponivel. Tem que estar livre e possuir tamanho suficiente, senão vai pro próximo
	while( l_current && ( l_current->free_block == 0 || l_current->size < a_size ) ) {
		// Atual não entrou nessas condições. Procura o próximo.
		l_current = l_current->next;
	}
	// Retorna um bloco livre, ou NULL se chegou no final da lista.
	return l_current;
}

void _split_free_block( block_t* a_block, size_t a_resize ) {	
	// "Cria" o novo bloco logo depois da parte de dados do bloco original.	
	block_t* l_new = (block_t*)(((char*)a_block) + BLOCK_SIZE + a_resize);		// C + 24 + 20 >> N
	// O tamanho deste novo bloco será o que sobrou de tamanho após o resize, deduzido também do tamanho do novo bloco
	l_new->size = a_block->size - ( a_resize + BLOCK_SIZE );					// 76 - (20 + 24) = 22
	// Não pode esquecer de MARCAR ele como um bloco que está livre.
	l_new->free_block = 1;
	// Finalmente, coloca no "meio" este bloco novo criado.
	// Para isso, primeiro aponta pro next do bloco anterior.
	l_new->next = a_block->next;
	// E agora, o bloco anterior aponta para o novo que foi criado.
	a_block->next = l_new;
	// Finalmente, coloca o tamanho novo no bloco que ja existia.
	a_block->size = a_resize;													// 20
}

void _merge_free_blocks() {
	
	// Curso para o bloco atual
	block_t* l_curr = s_blocks;

	// ENquanto o cursor não for nulo, avança mais na lista
	while( l_curr ) {
		// Recupera o proximo cursor, para ver se ele pode mesclar com o atual
		block_t* l_next = l_curr->next;
		// Se tanto o atual como o proximo estiverem marcados como free, mescla eles.
		if( l_curr && l_curr->free_block && l_next && l_next->free_block ) {
			// O novo tamanho, é o tamanho de bloco + o espaço alocado pelo bloco sendo "engolido".
			l_curr->size += BLOCK_SIZE + l_next->size;
			// Por fim, se livra da referencia para o bloco sendo removido
			l_curr->next = l_next->next;
			// MAS NÃO avança, pois o proximo pode ser mesclado também. Imagine o seguinte caso:
			// A B C -> AB C. Se avançar pro proximo aqui, nao vai fazer ABC.
		} else {
			// Senão, só avança para o proximo registro.
			l_curr = l_curr->next;
		}
	}
}

#ifdef DEBUG_LINUX
void print_blocks() {
	block_t* l_block = s_blocks;
	while( l_block ) {
		printf( "Bloco: %X\nsize = %d\nnext = %X\nfree = %d\n\n", l_block, l_block->size, l_block->next, l_block->free );
		l_block = l_block->next;
	}
}

void print_heap() {
	for( unsigned int i = 0; i < HEAP_SIZE; i += 16 ) {
		printf( "%2X %2X %2X %2X %2X %2X %2X %2X, %2X %2X %2X %2X %2X %2X %2X %2X\n",
			s_heap[i+0], s_heap[i+1], s_heap[i+2], s_heap[i+3],
			s_heap[i+4], s_heap[i+5], s_heap[i+6], s_heap[i+7],	
			s_heap[i+8], s_heap[i+9], s_heap[i+10], s_heap[i+11],
			s_heap[i+12], s_heap[i+13], s_heap[i+14], s_heap[i+15]
		);
	}
}
#endif//DEBUG_LINUX
