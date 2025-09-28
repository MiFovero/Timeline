#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Funcao auxiliar para trocar dois elementos de lugar
void swap(int *a, int *b) {
    int t = *a; // guarda temporariamente o valor de a
    *a = *b;    // atribui o valor de b para a
    *b = t;     // coloca o valor guardado em b
}

// QuickSort usando o primeiro elemento como pivo
int partFirst(int a[], int l, int r) {
    int p = a[l];       // pivo = primeiro elemento do subarray
    int i = l + 1;      // indice que vai da esquerda para direita
    int j = r;          // indice que vai da direita para esquerda
    while (i <= j) {    // enquanto os indices nao se cruzarem
        while (i <= j && a[i] <= p) i++; // encontra elemento maior que p
        while (j >= i && a[j] > p) j--;  // encontra elemento menor que p
        if (i <= j) {                     // se indices ainda nao se cruzaram
            swap(&a[i], &a[j]);           // troca os elementos
            i++;
            j--;
        }
    }
    swap(&a[l], &a[j]); // coloca pivo na posicao correta
    return j;           // retorna indice do pivo
}

void QuickSortFirst(int a[], int l, int r) {
    if (l < r) {                       // se houver mais de um elemento
        int p = partFirst(a, l, r);    // particiona o vetor
        QuickSortFirst(a, l, p - 1);   // aplica QuickSort na esquerda
        QuickSortFirst(a, p + 1, r);   // aplica QuickSort na direita
    }
}

// QuickSort usando o ultimo elemento como pivo
int partLast(int a[], int l, int r) {
    int p = a[r];       // pivo = ultimo elemento do subarray
    int i = l - 1;      // indice para controlar elementos menores que p
    for (int j = l; j < r; j++) { // percorre o subarray
        if (a[j] <= p) {           // se elemento <= p
            i++;
            swap(&a[i], &a[j]);    // coloca na parte esquerda
        }
    }
    swap(&a[i + 1], &a[r]);        // coloca pivo na posicao correta
    return i + 1;                   // retorna indice do pivo
}

void QuickSortLast(int a[], int l, int r) {
    if (l < r) {                     // se houver mais de um elemento
        int p = partLast(a, l, r);   // particiona o vetor
        QuickSortLast(a, l, p - 1);  // aplica QuickSort na esquerda
        QuickSortLast(a, p + 1, r);  // aplica QuickSort na direita
    }
}

// QuickSort usando pivo aleatorio
int partRand(int a[], int l, int r) {
    int k = l + rand() % (r - l + 1); // escolhe indice aleatorio
    swap(&a[k], &a[r]);               // troca com ultimo elemento
    return partLast(a, l, r);         // usa a logica do ultimo pivo
}

void QuickSortRand(int a[], int l, int r) {
    if (l < r) {
        int p = partRand(a, l, r);    // particiona aleatoriamente
        QuickSortRand(a, l, p - 1);   // aplica QuickSort na esquerda
        QuickSortRand(a, p + 1, r);   // aplica QuickSort na direita
    }
}

// QuickSort usando mediana de tres
int median3(int a[], int l, int r) {
    int m = (l + r) / 2;      // indice do elemento central
    if (a[l] > a[m]) swap(&a[l], &a[m]); // garante ordem entre l e m
    if (a[l] > a[r]) swap(&a[l], &a[r]); // garante ordem entre l e r
    if (a[m] > a[r]) swap(&a[m], &a[r]); // garante ordem entre m e r
    swap(&a[m], &a[r - 1]);   // coloca a mediana em r-1 para usar como pivo
    return a[r - 1];           // retorna valor do pivo
}

int partMedian(int a[], int l, int r) {
    int p = median3(a, l, r);  // escolhe pivo como mediana de tres
    int i = l;
    int j = r - 1;
    int stop = 0;
    while (!stop) {
        i++;
        while (a[i] < p) i++; // encontra elemento >= p da esquerda
        j--;
        while (a[j] > p) j--; // encontra elemento <= p da direita
        if (i >= j) {
            stop = 1;          // indices se cruzaram, parar
        } else {
            swap(&a[i], &a[j]); // troca elementos fora do lugar
        }
    }
    swap(&a[i], &a[r - 1]);    // coloca pivo na posicao correta
    return i;                  // retorna indice do pivo
}

void QuickSortMedian(int a[], int l, int r) {
    if (l + 10 <= r) {          // para subvetores maiores que 10
        int p = partMedian(a, l, r);
        QuickSortMedian(a, l, p - 1);
        QuickSortMedian(a, p + 1, r);
    } else {                     // para subvetores pequenos usa insertion sort
        for (int i = l + 1; i <= r; i++) {
            int key = a[i];
            int j = i - 1;
            while (j >= l && a[j] > key) {
                a[j + 1] = a[j];  // desloca elementos maiores
                j--;
            }
            a[j + 1] = key;       // insere elemento na posicao correta
        }
    }
}

// Funcoes para gerar vetores
void geraOrdenado(int *v, int n) {
    for (int i = 0; i < n; i++) v[i] = i; // valores de 0 a n-1
}

void geraQuaseOrdenado(int *v, int n) {
    geraOrdenado(v, n);             // primeiro gera vetor totalmente ordenado
    int qtd = n / 10;               // 10% dos elementos vao ser trocados
    for (int k = 0; k < qtd; k++) {
        int i = rand() % n;
        int j = rand() % n;
        swap(&v[i], &v[j]);         // troca elementos aleatoriamente
    }
}

void geraAleatorio(int *v, int n) {
    for (int i = 0; i < n; i++) v[i] = rand() % (n * 10); // numeros aleatorios
}

// Funcao que testa um QuickSort especifico
void testaTodos(const char *labelTipo,
                  void (*qs)(int*,int,int),
                  const char *nomeQS,
                  int *base,
                  int n) {
    int *copia = malloc(n * sizeof(int));  // cria copia do vetor original
    for (int i = 0; i < n; i++) copia[i] = base[i];

    printf("      -> usando %s\n", nomeQS);
    clock_t ini = clock();                 // inicia contagem de tempo
    qs(copia, 0, n - 1);                   // executa QuickSort
    clock_t fim = clock();                 // termina contagem

    double tempo = (double)(fim - ini) / CLOCKS_PER_SEC; // calcula tempo em segundos
    printf("         tempo: %.6f s\n", tempo);

    free(copia);  // libera memoria da copia
}

// Funcao que roda todos os QuickSorts para um tipo de vetor e tamanho
void rodaCaso(const char *tipo, void (*gera)(int*,int), int n) {
    int *vet = malloc(n * sizeof(int));
    gera(vet, n); // gera vetor do tipo especifico

    printf("Tamanho %5d | tipo de entrada: %s\n", n, tipo);
    testaTodos(tipo, QuickSortFirst,  "QuickSort - Primeiro pivo",   vet, n);
    testaTodos(tipo, QuickSortLast,   "QuickSort - Ultimo pivo",     vet, n);
    testaTodos(tipo, QuickSortRand,   "QuickSort - Pivo aleatorio",  vet, n);
    testaTodos(tipo, QuickSortMedian, "QuickSort - Mediana de tres", vet, n);
    printf("-------------------------------------------------\n");

    free(vet); // libera memoria
}

int main() {
    srand((unsigned)time(NULL));   // inicializa gerador de numeros aleatorios
    int tamanhos[] = {100, 1000, 10000};
    int qt = sizeof(tamanhos)/sizeof(tamanhos[0]);

    for (int i = 0; i < qt; i++) {
        int n = tamanhos[i];
        // roda testes para vetor ordenado, quase ordenado e aleatorio
        rodaCaso("ordenado",      geraOrdenado,     n);
        rodaCaso("quase ordenado",geraQuaseOrdenado,n);
        rodaCaso("aleatorio",     geraAleatorio,    n);
    }
    return 0;
}
