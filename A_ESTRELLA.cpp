#include <bits/stdc++.h>
using namespace std;
//Por Antonio Maldonado Pinzón
struct nodo{
	nodo* padre;
	int id;//id del nodo
	int estado[3][3];//matriz actual
	int x, y;//coordenadas del cero
	int h;//heuristica
	int nivel;//nivel del nodo
};
struct comp{//para comparar
	bool operator()(const nodo* a, const nodo* b) const{
		return (a->h + a->nivel) > (b->h + b->nivel);
	}
};
int fila[] = { 1, 0, -1, 0 };//para cambiar el cero
int columna[] = { 0, -1, 0, 1 };//para cambiar el cero
long idd=0;//contador de id de nodos 
int movimientos=0;//contador de movimientos
long nodos_creados=0;//contador nodos creados
int i,j,matriz1[3][3],metaa[3][3],cordx,cordy;
void leer();//lee la matriz del txt
void imprimir_matriz(int matriz[3][3]);//imprime una matriz 
nodo* nuevo_nodo(int matriz[3][3], int x, int y, int nueva_x,int nueva_y, int niv, nodo* pad);//funcion para crear un nuevo 
int se_puede(int x, int y);//para saber si se puede hacer el cambio
void imprimir_ruta(nodo* raiz);//imprimir la ruta final 
void a_estrella(int inicial[3][3], int x, int y,int meta[3][3]);//A estrella
void detectar_cero(int matriz[3][3]);//saca las coordenadas del cero 
int calcular_heuristica(int inicial[3][3],int meta[3][3]);//calcula la heuristica 
int num_man(int a,int posi,int posj);//calcula el numero manhattan de un numero 

int main(){
    leer();//leo la matriz inicial y la meta 
	detectar_cero(matriz1);//
	cout<<"Inicio"<<endl;
	a_estrella(matriz1, cordx, cordy, metaa);//inicializo a estrella
	cout<<"Movimientos:  "<<movimientos-1<<endl;
	cout<<"Nodos creados:  "<<nodos_creados<<endl;
	return 0;
}
void leer(){
freopen("in.txt","r",stdin);
	for(i=0;i<3;i++){//almaceno la matriz inicial
		for(j=0;j<3;j++)
	cin>>matriz1[i][j];
}	
	for(i=0;i<3;i++){	//almaceno la matriz final
		for(j=0;j<3;j++)
	cin>>metaa[i][j];
}
cout<<"Estado inicial"<<endl;
imprimir_matriz(matriz1);
cout<<endl<<"Meta"<<endl;
imprimir_matriz(metaa);
cout<<endl;
}
void imprimir_matriz(int matriz[3][3]){
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++)
		cout<<matriz[i][j]<<" ";
		cout<<endl<<endl;
	}
}
nodo* nuevo_nodo(int matriz[3][3], int x, int y, int nueva_x,int nueva_y, int niv, nodo* pad){//creo un nuevo nodo 
	nodo* nuevo = new nodo; 
	nuevo->padre = pad;//el padre del nuevo nodo va a ser el actual nodo
	memcpy(nuevo->estado, matriz, sizeof nuevo->estado);//le paso la matriz del padre al hijo
	swap(nuevo->estado[x][y], nuevo->estado[nueva_x][nueva_y]);//
	nuevo->h = 0;//inicializo la heuristica 
	nuevo->nivel = niv;//le paso el nivel del padre(luego le sumo 1 al nivel)
	nuevo->x = nueva_x;//la nueva coordenada x para el cero 
	nuevo->y = nueva_y;//la nueva coordenada y para el cero 
	nuevo->id=idd;//le pasa el id del padre(luego le sumo 1) 
	idd++;//le aumento el id
	nodos_creados++;//para contar los nodos 
	return nuevo;//retorno el nuevo nodo
}
int se_puede(int x, int y){//para saber si puedo mover el cero
	return (x >= 0 && x < 3 && y >= 0 && y < 3);
}
void imprimir_ruta(nodo* raiz){//para imprimir la ruta solucion
	if (raiz == NULL)
		return;
	imprimir_ruta(raiz->padre);
	imprimir_matriz(raiz->estado);
	cout<<endl<<"ID  "<<raiz->id<<endl;
	cout<<"h(n) "<<raiz->h<<endl;
	cout<<"f(n)  "<<raiz->h+raiz->nivel<<endl;
	printf("\n");
	movimientos++;//cuento los movimientos 
}
void a_estrella(int inicial[3][3], int x, int y,int meta[3][3]){
	priority_queue<nodo*, vector<nodo*>, comp> cola;//creo una cola de prioridad
	nodo* raiz = nuevo_nodo(inicial, x, y, x, y, 0, NULL);//creo el primer nodo (Arbol)
	raiz->h = calcular_heuristica(inicial, meta);//calculo la heuristica de la matriz inicial 
	cola.push(raiz);//meto al nodo raiz a la cola 
	while (!cola.empty()){//mientras la cola esté vacía
		nodo* minimo = cola.top();//tomo el primer nodo de la cola el cual tiene menor heuristica (va a almacenar la ruta solucion, El nodo minimo) 
		cola.pop();//saco los elementos de la cola;
		if (minimo->h == 0){//si la heuristica es cero es porque ya estamos en la meta 
			imprimir_ruta(minimo);
			return;
		}
		for (int i = 0; i < 4; i++){//para mover el cero y generar más estados 
			if (se_puede(minimo->x + fila[i], minimo->y + columna[i])){//si se puede mover el cero, se puede generar nuevos nodos 
				nodo* hijo = nuevo_nodo(minimo->estado, minimo->x,minimo->y, minimo->x + fila[i],minimo->y + columna[i],minimo->nivel + 1, minimo);
				hijo->h = calcular_heuristica(hijo->estado, meta);//creamos un nuevo nodo
				cola.push(hijo);//meto al hijo a la cola 
			}
		}
	}
}
void detectar_cero(int matriz[3][3]){//hayamos las coordenadas del cero
	for(i=0;i<3;i++)
	for(j=0;j<3;j++)
	if(matriz[i][j]==0){
		cordx=i;
		cordy=j;
	}
}
int calcular_heuristica(int inicial[3][3],int meta[3][3]){//calcula la heuristica de una matriz
	int acu=0,ai,aj;
for(i=0;i<3;i++){
	for(j=0;j<3;j++){
	ai=i;//guardo la coordenada i
	aj=j;//guardo la coordenada j
	if(inicial[i][j]!=0){	
	acu+=num_man(inicial[i][j],i,j);	//llamo a la funcion la cual me va a calcular la heuristica de ese numero
	j=aj;  //le devuelvo su valor a la coordenada j
	i=ai; //le devuelvo su valor a la coordenada i
	}
}
}			
return acu;//es la suma de las heuristicas de todos los numeros de la matriz
}
int num_man(int a,int posi,int posj){//calcula la heuristica de un numero
	int aux1=0,pi,pj;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			if(a==metaa[i][j]){
			pi=i;
			pj=j;
			break;
			}
		}
	}
	if(pi==posi && pj==posj){
		aux1=0;
	}else if(pi==posi){
		aux1=abs(pj-posj);
	}else if(pj==posj){
		aux1=abs(pi-posi);
	}else if(pi==pj && posi==posj){
		aux1=abs((abs(posi+posj))-(abs(pi+pj)));
	}else {
		aux1=abs((abs(pi-posi))+(abs(posj-pj)));
	}
	return aux1;//retorno la heuristica
}
