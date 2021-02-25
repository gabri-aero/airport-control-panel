/*PANEL DE CONTROL DE VUELOS*/

//Definición las librerías necesarias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <windows.h>

time_t t; //Estructuras de la librería time.h que permiten obtener la hora actual.
struct tm *tm;

FILE *file_variable; //Definición del puntero archivo.

//Primera definición de las funciones utilizadas.
void printp(char* p);
int plen(char *p);
void leer(char fichero[20],char* dato, int col, int linea);
char* p(char* p, int i);
void lectura();
void imprimir(char *avion, char *time, char *aerolinea, char *dest_orig, int i);
void cabecera(int c);
void reserva();
int conv(char* hora);
int actividad(int tavion, int delay);
void panel();
void info(int i);
void informar(int i, char mensaje[3][30], char mensaje2[7][7], int var1, int time, int limite, int* grupo, int lim, int delay);
void trestante(int tact, int treal);
int asignar(int i, int* grupo, int lim);
int comparar(int i, int* grupo, int lim);
int introAlerta();
int cancelar(int i);
void printDelay(int i);
void cambiarVuelo(int i, int* grupo, int lim, char mensaje[]);
void controlPanel();
void cambiarsubpantalla();
void cambiarpantalla();
void freeMemory();
void inicio();
void writeMemory();
void clearMemory();
void readMemory();
void totalVuelos();
int totLineas(char fichero[20]);
void inicioPista();
int compararDatos(char avion[]);
void empezar();
int calculoentrada(int rumbo_procedencia,int rumbo_outbound);
int entradanoest(int rumbo_procedencia,int rumbo_outbound);
void cajanegra(char modeloavion[],int procedencia,int outbound);
void entrada(int a);
int inicioTemp();
int temporizador(int tinic, int m, int s);

//Definición de las variables necesarias.
char* avionS;
char* horaS;
char* aerolineaS;
char* avionL;
char* horaL;
char* aerolineaL;
char* destino;
char* origen;
char* hf;
char* he;
int pantalla=1;
int subpantalla=1;
int ns;
int nl;
int* delay;
int* cancel;
int* mostrador;
int* puerta;
int* cinta;
int* alerta;
char mensajeFact[3][30]={"Facturando\t\t\t","Facturando. Cierre inminente\t","Facturacion cerrada\t\t\t\t"};
char mensajeMostr[7][7]={"1-15","16-30","31-45","46-60","61-75","76-90","91-105"};
char mensajeEmb[3][30]={"Embarcando\t\t\t","LAST CALL\t\t\t","Embarque cerrado\t\t\t\t"};
char mensajePuerta[7][7]={"A1","A2","B1","B2","C","D","E"};
char mensajeDesemb[3][30]={"Desembarcando\t\t\t","Desembarque completado\t\t","En tierra\t\t\t\t\t"};
char mensajeMaletas[3][30]={"Disponible\t\t\t","Disponible. Cierre inminente\t","Cinta cerrada\t\t\t\t\t"};
char mensajeCinta[7][7]={"1-2","3-4","5-6","7-8","9-10","11-12","13-14"};
int rumbo_outbound,limite_gota,limite_falsa,numero_entrada,rumbo_procedencia,b;
char modeloavion[10];
int empezado;
int paso=0,tinic,start=0;
int u,r,vueltas,m,s,s2,radial,radialgota,respuesta,t3=0;

int main(){
//Función principal del programa
	t=time(NULL); 
	tm=localtime(&t);
	srand(time(NULL)); //Tomamos el tiempo actual
	totalVuelos();
	reserva();
	inicio();
	lectura();
	panel();
	controlPanel();
}

void printp(char* p){
//Imprime la cadena de caracteres que se encuentran en la dirección de memoria introducida.
	int i=0;
	while(*(p+i)!='\0'){
		printf("%c", *(p+i));
		i++;
	}
}

int plen(char *p){
//Devuelve la longitud de cadena de caracteres en la dirección de memoria del puntero.
	int i=0;
	while(*(p+i)!='\0'){
		i++;
	}
	return(i);
}

char* p(char* p, int i){
//Devuelve la posición de memoria de la i-ésima cadena de caracteres.
	return(p+20*i);//Se establece en 20 la longitud máxima.
}

void leer(char fichero[20],char* dato, int col, int linea){ 
/*En el fichero introducido, lee la cadena de la columna y línea introducidas
y lo guarda en la posición de memoria de dato.*/
	int i,j=0,k=0;
	char line[200];
	file_variable = fopen(fichero, "r");
	while(j<=linea){
		fgets(line, sizeof(line), file_variable);
		if(j==linea){
			i=0;
			while(1){
				if(line[i]=='.'){ //Indica fin de línea.
					break;
				} 
				if(line[i]=='/'){ //Indica fin de columna.
					col--;
					i++;
				}
				if(col<0){
					break;
				}
				if(col==0){
					*(dato+k)=line[i];
					k++;
				}
				i++;	
			}
		}
		j++;
	}
	*(dato+k)='\0';
	fclose(file_variable);
}

void lectura(){ 
/*Se ejecuta mediante un bucle la función leer para todos las columnas y filas
de los dos archivos donde se guarda la información.*/
	int i;
	for(i=0;i<ns;i++){
		leer("salidas.txt", p(avionS,i),0,i);
		leer("salidas.txt", p(horaS,i),1,i);
		leer("salidas.txt", p(aerolineaS,i),2,i);
		leer("salidas.txt", p(destino,i),3,i);
		leer("salidas.txt", p(hf,i),4,i);
		leer("salidas.txt", p(he,i),5,i);
	}
	for(i=0;i<nl;i++){
		leer("llegadas.txt", p(avionL,i),0,i);
		leer("llegadas.txt", p(horaL,i),1,i);
		leer("llegadas.txt", p(aerolineaL,i),2,i);
		leer("llegadas.txt", p(origen,i),3,i);
	}
}

void imprimir(char *avion, char *time, char *aerolinea, char *dest_orig, int i){
//Imprime y organiza los valores de información que no cambian durante el desarrollo del programa
	printp(p(avion, i));
	printf("\t\t");
	printp(p(time, i)); 
	printf("\t");
	printp(p(aerolinea,i));
	if(plen(p(aerolinea,i))<=7){
		printf("\t");
	}
	printf("\t");
	printp(p(dest_orig,i));
	if(plen(p(dest_orig,i))<=7){
		printf("\t");
	}
	printf("\t");
}

void cabecera(int c){
//Imprime la cabecera del panel de información.
	char dato1[10],dato2[10];
	switch(pantalla){
		case 1:
			strcpy(dato1,"SALIDAS");
			strcpy(dato2,"DESTINO");
			break;
		case 2:
			strcpy(dato1,"LLEGADAS");
			strcpy(dato2,"ORIGEN");
	}
	//El nombre del aeropuerto ha sido asignado a modo de ejemplo.
	printf("PANEL DE INFORMACION %s - AEROPUERTO ADOLFO SUAREZ-MADRID-BARAJAS\n", dato1);
	printf("%02d:%02d %02d/%02d/%d\n\n", tm->tm_hour,tm->tm_min, tm->tm_mday, 1+tm->tm_mon, 1900+tm->tm_year);
	printf("AVION\t\t");
	printf("HORA\t");
	printf("AEROLINEA\t");
	printf("%s\t\t", dato2);
	printf("INFORMACION ");
	switch(pantalla){
		case 1:
			switch(subpantalla){
				case 1:
					printf("DE FACTURACION");
					printf("\t");
					printf("MOSTRADOR");
					break;
				case 2:
					printf("DE EMBARQUE");
					printf("\t\t");
					printf("PUERTA\t");
					break;
				case 3:
					printf("DE DESPEGUE\t\t\t");
					break;
			}
			break;
		case 2:
			switch(subpantalla){
				case 1:
					printf("DE MALETAS");
					printf("\t\t");
					printf("CINTA\t");
					break;
				case 2:
					printf("DE ATERRIZAJE\t");
					printf("PUERTA\t");
					break;
			}
			break;	
	}
	printf("\tALERTAS\n");
}

void reserva(){
//Reserva de memoria para los punteros.
	int i;
	avionS=(char*)malloc(sizeof(char)*ns*20);
	horaS=(char*)malloc(sizeof(char)*ns*20);
	aerolineaS=(char*)malloc(sizeof(char)*ns*20);
	avionL=(char*)malloc(sizeof(char)*nl*20);
	horaL=(char*)malloc(sizeof(char)*nl*20);
	aerolineaL=(char*)malloc(sizeof(char)*nl*20);
	destino=(char*)malloc(sizeof(char)*ns*20);
	origen=(char*)malloc(sizeof(char)*nl*20);
	he=(char*)malloc(sizeof(char)*ns*20);
	hf=(char*)malloc(sizeof(char)*ns*20);
//La función calloc realiza la misma función que malloc e inicializa esa memoria a 0.
	delay=(int*)calloc(ns+nl,sizeof(int));
	cancel=(int*)calloc(ns+nl,sizeof(int));
	mostrador=(int*)calloc(ns,sizeof(int));
	puerta=(int*)calloc(ns+nl,sizeof(int));
	cinta=(int*)calloc(nl,sizeof(int));
	alerta=(int*)calloc(ns+nl,sizeof(int));
}

int conv(char* hora){
//Convierte el puntero donde se guardan las horas al tiempo en minutos.
	int h,m;
	char horas[3], mins[3];
	horas[0]=*hora;
	horas[1]=*(hora+1);
	horas[2]='\0';
	mins[0]=*(hora+3);
	mins[1]=*(hora+4);
	mins[2]='\0';
	sscanf(horas,"%d",&h);
	sscanf(mins,"%d",&m);
	return(h*60+m);
}

int actividad(int tavion, int delay){
//Si no hay más información del vuelo se establece su actividad a 0.
	int treal;
	treal=tm->tm_hour*60+tm->tm_min;
	tavion=tavion+delay;
	if(treal<=tavion+5){
		return 1;
	}else{
		return 0;
	}
}

void panel(){
//Gestión de la información que aparece en pantalla en función de la pantalla actual.
	int i=0,j=0;
	switch(pantalla){
		case 1: //Panel de salidas.
			cabecera(pantalla);
			while(i<ns && j<15){ //Se imprimirá la información de los 15 vuelos siguientes.
				if(actividad(conv(p(horaS,i)), 0)){ //Si el vuelo no está activo no se imprime.
					imprimir(avionS,horaS,aerolineaS,destino,i);
					info(i);
					j++;
				}
				i++;
			}
			break;
		case 2: //Panel de llegadas.
			cabecera(pantalla);
			while(i<nl && j<15){//Se imprimirá la información de los 15 vuelos siguientes.
				if(actividad(conv(p(horaL,i))+120, 0)){//Si el vuelo no está activo no se imprime.
					imprimir(avionL,horaL,aerolineaL,origen,i);
					info(i);
					j++;	
				}
				i++;
			}
			break;
	}
	writeMemory();
}

void info(int i){
//Gestiona la información de los paneles que varía durante la ejecución del programa.
	int tact,treal;
	if(cancelar(i)){ //Si está cancelado no se imprime nada.
		printf("-----CANCELADO-----\n");
	}else{
		switch(pantalla){
			case 1: //Información de salidas.
				switch(subpantalla){
					case 1: //Información de facturación.
						informar(i,mensajeFact, mensajeMostr,atoi(p(hf,i)),conv(p(horaS,i)),30,mostrador, ns, *(delay+i));
						break;
					case 2: //Información de embarque.
						informar(i, mensajeEmb, mensajePuerta, atoi(p(he,i)),conv(p(horaS,i)),5,puerta, ns+nl, *(delay+i));
						break;
					case 3: //Información de despegue.
						treal=tm->tm_hour*60+tm->tm_min;
						tact=conv(p(horaS,i))+*(delay+i);
						if(tact>treal){
							trestante(tact,treal);
						}else{
							printf("Despegando...\t\t\t\t\t");
						}
						break;
				}
				printDelay(i);
				break;
			case 2: //Información de llegadas.
				switch(subpantalla){
					case 1: //Información de cinta.
						informar(i,mensajeMaletas, mensajeCinta,110,conv(p(horaL,i))+120,20,cinta, nl, *(delay+i+ns));
						break;
					case 2: //Información de desembarque.
						informar(i+ns,mensajeDesemb, mensajePuerta, 125, conv(p(horaL,i))+120,90, puerta, ns+nl, *(delay+i+40));
						break;
				}
				printDelay(i+ns);
				break;
		}
	printf("\n");
	}
}

void informar(int i, char mensaje[3][30], char mensaje2[7][7], int var1, int time, int limite, int* grupo, int lim, int delay){
//Dependiendo del grupo introducido se imprime una información u otra. 
	int tdesp, tact, treal, trest, tlim, mrest, hrest,num, asig;
	int antelacion;
	antelacion=var1;
	tdesp=time;
	tact=tdesp-antelacion+delay;
	treal=tm->tm_hour*60+tm->tm_min;
	tlim=tdesp-limite; 
	trest=tlim-treal;
	if(tact>treal){
		trestante(tact,treal);
	}else if(trest>0){
		if(trest>10){
			printf("%s", mensaje[0]);
		}else{
			printf("%s", mensaje[1]);
		}
	}else{
		printf("%s", mensaje[2]);
	}
	if(trest>0 && tact<=treal){ //Se da la imformación de la puerta, cinta, etc. concreta.
		asig=asignar(i,grupo, lim);
		if(asig!=8){ //Se ha asignado correctamente la puerta, la cinta...
			printf("%s\t\t", mensaje2[asig-1]);
		}else{
			printf("EN ESPERA\t");
		}
	}else if(*(grupo+i)!=0){ //Ya no se da más información de la puerta, cinta, etc.
		*(grupo+i)=0;
	}
}

void trestante(int tact, int treal){
	//Devuelve el tiempo restante en minutos dependiendo de los valores de tiempo introducidos.
	int trest=tact-treal;
	int mrest=trest%60;
	int hrest=(trest-mrest)/60;
	if(hrest>0){
		printf("En %d h y %d min\t\t\t\t\t", hrest, mrest);
	}else if(mrest==0){
		printf("En %d h \t\t\t\t\t", hrest);
	}else if(hrest==0){
		printf("En %d min\t\t\t\t\t", mrest);
	}
}

int asignar(int i, int* grupo, int lim){
	//Asigna al elemento del grupo un valor disponible (entre 1 y 7) y distinto de 0.
	int num, k, iter=0;
	if(*(grupo+i)==0){
		do{
			num=rand()%7+1;
			*(grupo+i)=num;
			iter++;
			if(iter>200){ //Un máximo de 200 iteraciones.
				num=8; //En caso de asignación fallida devuelve 8.
				break;
			}
		}while(comparar(i, grupo, lim)); //Compara que ese valor no esté asignado a otros elementos del grupo.
	}else{
		num=*(grupo+i);
	}
	*(grupo+i)=num;
	return num;
}

int comparar(int i, int* grupo, int lim){
	//La función ya fue explicada previamente en asignar.
	int j=0,k=0;
	while(j<lim){
		if(*(grupo+i)==*(grupo+j)){
			k++;
		}
		j++;
	}
	k--;
	return k;
}

int introAlerta(){
//Gestión de la introducción de las alertas.
	printf("\nCONFIGURACI%cN DE ALARMAS\n",224);
	int i, j=0, k;
	printf("Introduce el vuelo: "); //Se introduce la posición del vuelo en la lista del panel.
	scanf("%d", &i);
	i--;
	if(pantalla==2){
		i=i+ns;
		j=ns;
	}
	for(j;j<=i; j++){
		if(i<ns){
			if(actividad(conv(p(horaS,j)), *(delay+j))==0){
				i++;
			}
		}else{
			if(actividad(conv(p(horaL,j-ns))+120, *(delay+j))==0){
				 i++;
			}
		}
	}
/*Se ha realizado el ajuste entre el valor introducido y el índice asignado a ese vuelo
por el programa.*/
	printf("Introduce el c%cdigo de alerta: ",162);
	scanf("%d", &j);
	switch(j){
		case 0:
			//Anulación de alertas.
			printf("Se anular%cn todas las alertas de este vuelo",160);
			*(cancel+i)=0;
			*(delay+i)=0;
			Sleep(1000);
			break;
		case 1:
			//Introducción de retrasos.
			printf("Introduce los minutos de retraso de este vuelo: ");
			scanf("%d", &k); //Minutos de retraso.
			*(delay+i)=k;
			break;
		case 2:
			//Cancelación del vuelo
			printf("Se cancelar%c este vuelo",160);
			*(cancel+i)=1;
			Sleep(1000);
			break;
		case 3:
			//Cambio de la puerta asignada al vuelo.
			cambiarVuelo(i, puerta, ns+nl, "puerta");
			break;
		case 4:
			if(pantalla==1){ //Para el panel de salidas cambio de mostrador.
				cambiarVuelo(i, mostrador, ns, "mostrador");
			}
			if(pantalla==2){ //Para el panel de llegadas cambio de cinta.
				cambiarVuelo(i-ns, cinta, nl, "cinta");
			}
			break;
	}
	system("cls");
	panel();
	Sleep(1000);
}

int cancelar(int i){
//Devuelve 1 si el vuelo está cancelado o 0 si sigue habilitado.
	if(pantalla==1){
		if(*(cancel+i)==1){
			return 1;
		}else{
			return 0;
		}
	}else if(pantalla==2){
		if(*(cancel+i+ns)==1){
			return 1;
		}else{
			return 0;
		}
	}
}

void printDelay(int i){
//Si hay algún retraso muestra la alerta por pantalla.
	if(*(delay+i)!=0){
		printf("Retraso: %d min", *(delay+i));
	}
}

void cambiarVuelo(int i, int* grupo, int lim, char mensaje[]){
//Asigna el elemento del grupo que introduzca el usuario por teclado si está disponible (línea 559).
	int k;
	printf("Introduce el n%cmero de %s al que desea asignar este vuelo: ",163, mensaje);
	scanf("%d", &k);
	*(grupo+i)=k;
	if(comparar(i, grupo, lim)){
		printf("ERROR - %s no disponible\a", mensaje);
		Sleep(3000);
		*(grupo+i)=0;
	}else{
		printf("Asignaci%cn de %s correcta",162, mensaje);
	}
}

void controlPanel(){
//Gestión de los cambios de pantalla en función del tiempo, procesos, cambio manual, etc.
	int dif12, sec12;
	while(1){
		sec12=(tm->tm_sec)-(tm->tm_sec%5);
		t=time(NULL);
		tm=localtime(&t);
		dif12=(tm->tm_sec)-sec12;
		 //Actualización del panel de información cada 5 segundos.
		if((dif12>=5||dif12<0)){
			system("cls");
			cambiarsubpantalla();
			panel();
			sec12=tm->tm_sec;
		}
		if(GetKeyState(VK_MENU) & 0x80000000){
			//Introducir alterta en los paneles.
			introAlerta();
		}      
		if(GetKeyState(VK_CONTROL) & 0x80000000){
		//La tecla CONTROL permite el cambio de pantalla.
			cambiarpantalla();
			Sleep(200);
		}
		if(GetKeyState(0x43) & 0x80000000){
		//La tecla 'c' permite la salida del programa liberando la memoria asignada a los punteros.
			freeMemory();
			exit(0);
		}
	}
	//Las actualizaciones no se llevarán a cabo si se esta realizando una entrada por teclado.
}

void cambiarsubpantalla(){
	subpantalla++;
	if((pantalla==1 && subpantalla>3)||(pantalla==2 && subpantalla>2)){
		subpantalla=1;
	}
}

void cambiarpantalla(){
	pantalla++;
	if(pantalla>2){
		pantalla=1;
	}
	system("cls");
	panel();
}

void freeMemory(){
//Liberación de la memoria asignada a los punteros.
	free(avionS);
	free(horaS);
	free(aerolineaS);
	free(destino);
	free(avionL);
	free(horaL);
	free(aerolineaL);
	free(origen);
	free(he);
	free(hf);
	free(delay);
	free(cancel);
	free(mostrador);
	free(puerta);
	free(cinta);
	free(alerta);
}

void inicio(){
//Si se desea se podrá cargar o borrar la información previa con los cambios realizados.
	int i;
	char c;
	do{
		printf("Desea recuperar los datos guardados en la memoria? \n");
		printf("1. SI\n");
		printf("2. NO\n");
		scanf("%d", &i);
	}while(i!=1 && i!=2);
	switch(i){
		case 1:
			printf("Se tomaran los datos de la memoria.");
			readMemory();
			break;
		case 2:
			printf("Se borraran los datos de la memoria.");
			clearMemory();
			break;
		default:
			printf("Valor incorrecto.\a");
			Sleep(1000);
			system("cls");
			break;
	}
	Sleep(1000);
	system("cls");
}

void writeMemory(){
//Función que permite la escritura en la memoria de los cambios efectuados en los grupos.
	int i;
	file_variable=fopen("memoria.txt", "w");
	for(i=0;i<ns+nl;i++){
		fputc(*(puerta+i)+'0', file_variable);
		if(i<ns){
			fputc(*(mostrador+i)+'0', file_variable);
		}else{
			fputc(*(cinta+i-ns)+'0', file_variable);
		}
		fputc('\n', file_variable);
	}
	fclose(file_variable);
}

void clearMemory(){
//Vacía la memoria, elimina los datos guardados.
	int i;
	file_variable=fopen("memoria.txt", "w");
	for(i=0;i<ns+nl;i++){
		fputc('0', file_variable);
		fputc('0', file_variable);
		fputc('\n', file_variable);
	}
	fclose(file_variable);
}

void readMemory(){
//Lectura de la memoria, asigna los datos guardados a las variables correspondientes.
	int i;
	char line[8];
	file_variable=fopen("memoria.txt", "r");
	for(i=0;i<ns+nl;i++){
		fgets(line, sizeof(line), file_variable);
		*(puerta+i)=line[0]-48;
		if(i<ns){
			*(mostrador+i)=line[1]-48;
		}else{
			cinta[i-ns]=line[1]-48;
		}
		
	}
}

void totalVuelos(){
	ns=totLineas("salidas.txt");
	nl=totLineas("llegadas.txt");
}

int totLineas(char fichero[20]){
//El número total de vuelos vendrá determinado por el número total de las líneas de cada fichero.
	char s[100];
	int i=0;
	file_variable=fopen(fichero,"r");
/*Cargamos sucesivamente las líneas de los ficheros. El bucle finaliza cuando la longitud de la
línea es 0*/
	do{
		s[0]='\0';
		fgets(s, sizeof(s), file_variable);
		if(strlen(s)!=0){
			i++;
		}
	}while(strlen(s)!=0);
	fclose(file_variable);
	return(i); //Devuelve el total de líneas.
}




