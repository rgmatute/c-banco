#include "time.h"
#include "stdio.h"
#include "conio.h"
#include "cstdlib"
#include "string.h"
#include "ctype.h"
//-----------------------FECHA DEL SISTEMA------
time_t t; 
struct tm *ahora; 
//----------------------------------------------

struct FECHA {
	int dd,mm,aaa;
	int h,m,s;
};
struct DIRECCION{
	char calleprin[30];
	char num[10];
	char callesec[30];
};
struct TRANSACCION{
	struct FECHA fec_transaccion;	//Sistema
	int tipo;
		union{
		struct{
			float valor;
		}dep;
		struct{
			float valor;
		}ret;
		struct{
			float valor;
		}tran;
	};
};
struct datos {
	int num_cuenta;	 		//Automática
	int num_transaccion;	 //Automática
	char cedula[11];
	char nombre[30];
	char apellido[30];
	struct DIRECCION dir_vivienda;
	char telefono[11];
	struct FECHA fec_nacimiento;	//Validado
	struct FECHA fec_ingreso;	  //Sistema
	float abono_inicial;
	float saldo;
	int Estado;		//  1: (A)ctivo  ó  0: (I)nactivo
	int tipo_cuenta; // 1: ahorro	2: corriente
	union{
		struct {
			struct TRANSACCION movi;
		}ahorro;
		struct {
			struct TRANSACCION movi;
		}corriente;
	};
};
typedef struct datos DATO;

bool logica=false;
char menu_principal();
void deposito();
void retiro();
void transferencia();
void transferencia_pross(int &num2 , float &valor);
void consultar();
void salir();
void presentar();
void saldos();
int validar_transaccion(int &e);

int main(void){
	char op,op2;
	do{
		op=menu_principal();
		switch(op){
			case '1':
				system("cls");
				deposito();
			break;
			case '2':
				system("cls");
				retiro();
			break;
			case '3':
				system("cls");
				transferencia();
			break;
			case '4':
				system("cls");
				consultar();
			break;
			case '5':
				system("cls");
				presentar();
			break;
			case '6':
				system("cls");
				saldos();
			break;
			case '7':
				system("cls");
				salir();
			break;
			default:
				printf("\n\n\n\t\t\tOpcion [%c] invalido !",op); getch();
			break;
		} 
	}while(op!='7');
	
	return 0;
}

char menu_principal(){
	char op;
	system("cls");
	printf("\n* * Menu Transaccion * *");
	printf("\n (1)Deposito              :");
	printf("\n (2)Retiro                :");
	printf("\n (3)Transferencia         :");
	printf("\n (4)Consultar movimientos :");
	printf("\n (5)Ultimos movimientos   :");
	printf("\n (6)Saldo de Cuenta       :");
	printf("\n (7)SALIR");
	printf("\n Opcion --> ");fflush(stdin);
	op = getch();
	return op;
}

int validar_transaccion(int &e){
	FILE *fich;DATO d;
	if ((fich = fopen("OPERACION.dat", "r")) == NULL)  { 
		printf("Fichero no existe\n " );
	} else {
	fread(&d,sizeof(d),1,fich);
	while(! feof(fich)){
		if(e == d.num_transaccion){
			while(e == d.num_transaccion){
				e = 9999+(rand()%99999); /*----No. Transaccion Automatca----*/
			}
		}
		fread(&d,sizeof(d),1,fich);
	}
	fclose(fich);
}
	return e;
}

void deposito(){
	FILE *fich , *fich2,*fich3;
	DATO temp,e;
	int num;
	//-----------------------FECHA DEL SISTEMA-------
	time(&t); 
	ahora = localtime(&t);
	//-----------------------------------------------
	printf("\n\t\t * DEPOSITO cta. *");
	if ((fich = fopen("ADMINISTRACION.dat", "r")) == NULL)  { 
		printf("Fichero no existe\n " );
	} else
	if ((fich2 = fopen("aux_.dat", "a")) == NULL)  { 
		printf("Fichero no existe\n " );
		fclose(fich);
	} else 
	if ((fich3 = fopen("OPERACION.dat", "a")) == NULL)  { 
		printf("Fichero no existe\n " );
		fclose(fich2);
	} else {
		do{
		printf("\n\n Ingrese No. Cta. : ");fflush(stdin);
		}while(!scanf("%i",&num));
		fread (&e, sizeof(e), 1, fich);
		while (! feof(fich)) { 
			if(num==e.num_cuenta){
				if(e.Estado == 1){
					logica=true;
					temp.num_transaccion = 9999+(rand()%99999); /*----No. Transaccion Automatca----*/
					//----------------------------------------------------------
					temp.num_transaccion=validar_transaccion(temp.num_transaccion);
					//----------------------------------------------------------
					
					printf("\n\tTransaccion No. %i",temp.num_transaccion);
					switch(e.tipo_cuenta){
						case 1:
							temp.num_cuenta=e.num_cuenta;
							//-----------------------FECHA DEL SISTEMA--------------
							temp.ahorro.movi.fec_transaccion.dd = ahora->tm_mday;
							temp.ahorro.movi.fec_transaccion.mm = ahora->tm_mon + 1;
							temp.ahorro.movi.fec_transaccion.aaa = ahora->tm_year +1900;
							temp.ahorro.movi.fec_transaccion.h = ahora->tm_hour;
							temp.ahorro.movi.fec_transaccion.m = ahora->tm_min;
							temp.ahorro.movi.fec_transaccion.s = ahora->tm_sec;
							//------------------------------------------------------
							do{
								printf("\n\n Cantidad a Depositar: $ ");
							}while(!scanf("%f",&temp.ahorro.movi.dep.valor)||temp.ahorro.movi.dep.valor<=0);
							e.saldo = e.saldo + temp.ahorro.movi.dep.valor;
							strcpy(temp.nombre,e.nombre);
							strcpy(temp.apellido,e.apellido);
							temp.saldo = e.saldo;
							temp.ahorro.movi.tipo=1;
							temp.tipo_cuenta=1;
						break;
						case 2:
							temp.num_cuenta=e.num_cuenta;
							//-----------------------FECHA DEL SISTEMA--------------
							temp.corriente.movi.fec_transaccion.dd = ahora->tm_mday;
							temp.corriente.movi.fec_transaccion.mm = ahora->tm_mon + 1;
							temp.corriente.movi.fec_transaccion.aaa = ahora->tm_year +1900;
							temp.corriente.movi.fec_transaccion.h = ahora->tm_hour;
							temp.corriente.movi.fec_transaccion.m = ahora->tm_min;
							temp.corriente.movi.fec_transaccion.s = ahora->tm_sec;
							do{
								printf("\n\n Cantidad a Depositar: $ ");
							}while(!scanf("%f",&temp.corriente.movi.dep.valor)||temp.corriente.movi.dep.valor<=0);
							e.saldo = e.saldo + temp.corriente.movi.dep.valor;
							strcpy(temp.nombre,e.nombre);
							strcpy(temp.apellido,e.apellido);
							temp.saldo = e.saldo;
							temp.corriente.movi.tipo=1;
							temp.tipo_cuenta=1;
						break;
					}
					fwrite(&temp, sizeof(temp), 1, fich3);//-----------------------------------
				}else{
					printf("\n\t La cta. esta INACTIVA. . .");
				}
				fwrite(&e, sizeof(e), 1, fich2);//-----------------------------------
			}else{
//				printf("\n\t EL No. cta no existe. . . .");
				fwrite(&e, sizeof(e), 1, fich2);//-----------------------------------
			}
		fread (&e, sizeof(e), 1, fich);
	}
		fclose(fich);
		fclose(fich2);
		fclose(fich3);
		remove("ADMINISTRACION.dat");//-----------------------------
		rename("aux_.dat","ADMINISTRACION.dat");
	}
	
	getch();
}

void retiro(){
	FILE *fich , *fich2,*fich3;
	DATO temp,e;
	int num;
	//-----------------------FECHA DEL SISTEMA-------
	time(&t); 
	ahora = localtime(&t);
	//-----------------------------------------------
	printf("\n\t\t * RETIRO cta. *");
	if ((fich = fopen("ADMINISTRACION.dat", "r")) == NULL)  { 
		printf("Fichero no existe\n " );
	} else
	if ((fich2 = fopen("aux_.dat", "a")) == NULL)  { 
		printf("Fichero no existe\n " );
		fclose(fich);
	} else 
	if ((fich3 = fopen("OPERACION.dat", "a")) == NULL)  { 
		printf("Fichero no existe\n " );
		fclose(fich2);
	} else {
		do{
		printf("\n\n Ingrese No. Cta. : ");fflush(stdin);
		}while(!scanf("%i",&num));
		fread (&e, sizeof(e), 1, fich);
		while (! feof(fich)) {
			if(num==e.num_cuenta){
				if(e.Estado == 1){
					if(e.saldo==0.00){ printf("\n\t\t No tiene Saldo Disponible . .");}else{
					temp.num_transaccion = 9999+(rand()%99999); /*----No. Transaccion Automatca----*/
					//----------------------------------------------------------
					temp.num_transaccion=validar_transaccion(temp.num_transaccion);
					//----------------------------------------------------------
					
					printf("\n\tTransaccion No. %i",temp.num_transaccion);
					switch(e.tipo_cuenta){
						case 1:
							temp.num_cuenta=e.num_cuenta;
							//-----------------------FECHA DEL SISTEMA--------------
							temp.ahorro.movi.fec_transaccion.dd = ahora->tm_mday;
							temp.ahorro.movi.fec_transaccion.mm = ahora->tm_mon + 1;
							temp.ahorro.movi.fec_transaccion.aaa = ahora->tm_year +1900;
							temp.ahorro.movi.fec_transaccion.h = ahora->tm_hour;
							temp.ahorro.movi.fec_transaccion.m = ahora->tm_min;
							temp.ahorro.movi.fec_transaccion.s = ahora->tm_sec;
							//------------------------------------------------------
							do{
								printf("\n\n Cantidad a Retirar: $ ");
							}while(!scanf("%f",&temp.ahorro.movi.ret.valor)||temp.ahorro.movi.ret.valor<=0||temp.ahorro.movi.ret.valor>e.saldo);
							e.saldo = e.saldo - temp.ahorro.movi.ret.valor;
							strcpy(temp.nombre,e.nombre);
							strcpy(temp.apellido,e.apellido);
							temp.saldo = e.saldo;
							temp.ahorro.movi.tipo=2;
							temp.tipo_cuenta=1;
						break;
						case 2:
							temp.num_cuenta=e.num_cuenta;
							//-----------------------FECHA DEL SISTEMA--------------
							temp.corriente.movi.fec_transaccion.dd = ahora->tm_mday;
							temp.corriente.movi.fec_transaccion.mm = ahora->tm_mon + 1;
							temp.corriente.movi.fec_transaccion.aaa = ahora->tm_year +1900;
							temp.corriente.movi.fec_transaccion.h = ahora->tm_hour;
							temp.corriente.movi.fec_transaccion.m = ahora->tm_min;
							temp.corriente.movi.fec_transaccion.s = ahora->tm_sec;
							do{
								printf("\n\n Cantidad a Retirar: $ ");
							}while(!scanf("%f",&temp.corriente.movi.ret.valor)||temp.corriente.movi.ret.valor<=0);
							e.saldo = e.saldo - temp.corriente.movi.ret.valor;
							strcpy(temp.nombre,e.nombre);
							strcpy(temp.apellido,e.apellido);
							temp.saldo = e.saldo;
							temp.corriente.movi.tipo=2;
							temp.tipo_cuenta=2;
						break;
					}
					fwrite(&temp, sizeof(temp), 1, fich3);//-----------------------------------
				}//......................................
				}else{
					printf("\n\t La cta. esta INACTIVA. . .");
				}
				fwrite(&e, sizeof(e), 1, fich2);//-----------------------------------
			}else{
//				printf("\n\t EL No. cta no existe. . . .");
				fwrite(&e, sizeof(e), 1, fich2);//-----------------------------------
			}
		fread (&e, sizeof(e), 1, fich);
	}
		fclose(fich);
		fclose(fich2);
		fclose(fich3);
		remove("ADMINISTRACION.dat");//-----------------------------
		rename("aux_.dat","ADMINISTRACION.dat");
	}
	
	
	getch();
}

void transferencia(){
	FILE *fich , *fich2,*fich3;
	DATO temp,e;
	int num1,num2;float valor;
	//-----------------------FECHA DEL SISTEMA-------
	time(&t); 
	ahora = localtime(&t);
	//-----------------------------------------------
	printf("\n\t\t * TRANSFERENCIA de DINERO. *");
	if ((fich = fopen("ADMINISTRACION.dat", "r")) == NULL)  { 
		printf("Fichero no existe\n " );
	} else
	if ((fich2 = fopen("aux_.dat", "a")) == NULL)  { 
		printf("Fichero no existe\n " );
		fclose(fich);
	} else 
	if ((fich3 = fopen("OPERACION.dat", "a")) == NULL)  { 
		printf("Fichero no existe\n " );
		fclose(fich2);
	} else {
		do{
		printf("\n\n Ingrese No. Cta. de la cual va a TRANSFERIR : ");fflush(stdin);
		}while(!scanf("%i",&num1));
		fread (&e, sizeof(e), 1, fich);
		while (! feof(fich)) {
			if(num1==e.num_cuenta){
				if(e.Estado == 1){
					if(e.saldo==0.00){ printf("\n\t\t No tiene Saldo Disponible . .");}else{
					do{
					printf("\n Ingrese No. Cta. a la cual le va a TRANSFERIR : ");fflush(stdin);
					}while(!scanf("%i",&num2));
					temp.num_transaccion = 9999+(rand()%99999); /*----No. Transaccion Automatca----*/
					//----------------------------------------------------------
					temp.num_transaccion=validar_transaccion(temp.num_transaccion);
					//----------------------------------------------------------
					
					printf("\n\tTransaccion No. %i",temp.num_transaccion);
					switch(e.tipo_cuenta){
						case 1:
							temp.num_cuenta=e.num_cuenta;
							//-----------------------FECHA DEL SISTEMA--------------
							temp.ahorro.movi.fec_transaccion.dd = ahora->tm_mday;
							temp.ahorro.movi.fec_transaccion.mm = ahora->tm_mon + 1;
							temp.ahorro.movi.fec_transaccion.aaa = ahora->tm_year +1900;
							temp.ahorro.movi.fec_transaccion.h = ahora->tm_hour;
							temp.ahorro.movi.fec_transaccion.m = ahora->tm_min;
							temp.ahorro.movi.fec_transaccion.s = ahora->tm_sec;
							//------------------------------------------------------
							do{
								printf("\n\n Cantidad a Transferir: $ ");
							}while(!scanf("%f",&temp.ahorro.movi.tran.valor)||temp.ahorro.movi.tran.valor<=0||temp.ahorro.movi.tran.valor>e.saldo);
							e.saldo = e.saldo - temp.ahorro.movi.tran.valor;
							strcpy(temp.nombre,e.nombre);
							strcpy(temp.apellido,e.apellido);
							temp.saldo = e.saldo;
							temp.ahorro.movi.tipo=3;
							temp.tipo_cuenta=1;
							valor=temp.ahorro.movi.tran.valor;
						break;
						case 2:
							temp.num_cuenta=e.num_cuenta;
							//-----------------------FECHA DEL SISTEMA--------------
							temp.corriente.movi.fec_transaccion.dd = ahora->tm_mday;
							temp.corriente.movi.fec_transaccion.mm = ahora->tm_mon + 1;
							temp.corriente.movi.fec_transaccion.aaa = ahora->tm_year +1900;
							temp.corriente.movi.fec_transaccion.h = ahora->tm_hour;
							temp.corriente.movi.fec_transaccion.m = ahora->tm_min;
							temp.corriente.movi.fec_transaccion.s = ahora->tm_sec;
							do{
								printf("\n\n Cantidad a Transferir: $ ");
							}while(!scanf("%f",&temp.corriente.movi.tran.valor)||temp.corriente.movi.tran.valor<=0);
							e.saldo = e.saldo - temp.corriente.movi.tran.valor;
							strcpy(temp.nombre,e.nombre);
							strcpy(temp.apellido,e.apellido);
							temp.saldo = e.saldo;
							temp.corriente.movi.tipo=3;
							temp.tipo_cuenta=2;
							valor=temp.corriente.movi.tran.valor;
						break;
					}
					fwrite(&temp, sizeof(temp), 1, fich3);//-----------------------------------
				}
				}else{
//					printf("\n\t La cta. esta INACTIVA. . .");
				}
				fwrite(&e, sizeof(e), 1, fich2);//-----------------------------------
			}else{
//				printf("\n\t EL No. cta no existe. . . .");
				fwrite(&e, sizeof(e), 1, fich2);//-----------------------------------
			}
		fread (&e, sizeof(e), 1, fich);
	}
		fclose(fich);
		fclose(fich2);
		fclose(fich3);
		remove("ADMINISTRACION.dat");//-----------------------------
		rename("aux_.dat","ADMINISTRACION.dat");
	}
	transferencia_pross(num2,valor);
	
	getch();
}

void transferencia_pross(int &num2 , float &valor){
	FILE *fich,*fich2;
	DATO e;
	if ((fich = fopen("ADMINISTRACION.dat", "r")) == NULL)  { 
		printf("Fichero no existe\n " );
	} else
	if ((fich2 = fopen("aux2_.dat", "a")) == NULL)  { 
		printf("Fichero no existe\n " );
		fclose(fich);
	} else {
		fread (&e, sizeof(e), 1, fich);
		while (! feof(fich)) { 
			if(num2==e.num_cuenta){
				e.saldo = e.saldo + valor;
				fwrite(&e, sizeof(e), 1, fich2);//-----------------------------------
			}else{
				fwrite(&e, sizeof(e), 1, fich2);//-----------------------------------
			}
		fread (&e, sizeof(e), 1, fich);
	}
		fclose (fich);
		fclose (fich2);
		remove("ADMINISTRACION.dat");//-----------------------------
		rename("aux2_.dat","ADMINISTRACION.dat");
	}
	
}

void consultar(){
	FILE *fich; DATO e;
	int num;
	printf("\n\t\t * CONSULTAR TRANSACCION. *\n");
	if ((fich = fopen("OPERACION.dat", "r")) == NULL)  { 
		printf("Fichero no existe\n " );
	} else {
	do{
		printf("\n\n Ingrese No. Cta. : ");fflush(stdin);
	}while(!scanf("%i",&num));
	fread(&e,sizeof(e),1,fich);
	while(! feof(fich)){
		if(num == e.num_cuenta){
			printf("\n________________________________");
			printf("\n\t No. Transaccion: %i",e.num_transaccion);	
			printf("\n No. Cta........: %i",e.num_cuenta);
			printf("\n Cliente........: %s %s",e.nombre,e.apellido);
			switch(e.tipo_cuenta){
				case 1:
					switch(e.ahorro.movi.tipo){
						case 1:
							printf("\n Fecha..........: %02d/%02d/%02d Hora: %02d:%02d:%02d",e.ahorro.movi.fec_transaccion.dd,e.ahorro.movi.fec_transaccion.mm,e.ahorro.movi.fec_transaccion.aaa,e.ahorro.movi.fec_transaccion.h,e.ahorro.movi.fec_transaccion.m,e.ahorro.movi.fec_transaccion.s);
							printf("\n DEPOSITO.......: $%.2f",e.ahorro.movi.dep.valor);
							printf("\n Saldo..........: $%.2f",e.saldo);
						break;
						case 2:
							printf("\n Fecha..........: %02d/%02d/%02d Hora: %02d:%02d:%02d",e.ahorro.movi.fec_transaccion.dd,e.ahorro.movi.fec_transaccion.mm,e.ahorro.movi.fec_transaccion.aaa,e.ahorro.movi.fec_transaccion.h,e.ahorro.movi.fec_transaccion.m,e.ahorro.movi.fec_transaccion.s);
							printf("\n RETIRO.........: $%.2f",e.ahorro.movi.ret.valor);
							printf("\n Saldo..........: $%.2f",e.saldo);
						break;
						case 3:
							printf("\n Fecha..........: %02d/%02d/%02d Hora: %02d:%02d:%02d",e.ahorro.movi.fec_transaccion.dd,e.ahorro.movi.fec_transaccion.mm,e.ahorro.movi.fec_transaccion.aaa,e.ahorro.movi.fec_transaccion.h,e.ahorro.movi.fec_transaccion.m,e.ahorro.movi.fec_transaccion.s);
							printf("\n TRANSFERENCIA..: $%.2f",e.ahorro.movi.tran.valor);
							printf("\n Saldo..........: $%.2f",e.saldo);
						break;
					}
				break;
				case 2:
					switch(e.corriente.movi.tipo){
						case 1:
							printf("\n Fecha..........: %02d/%02d/%02d Hora: %02d:%02d:%02d",e.corriente.movi.fec_transaccion.dd,e.corriente.movi.fec_transaccion.mm,e.corriente.movi.fec_transaccion.aaa,e.corriente.movi.fec_transaccion.h,e.corriente.movi.fec_transaccion.m,e.corriente.movi.fec_transaccion.s);
							printf("\n DEPOSITO.......: $%.2f",e.corriente.movi.dep.valor);
							printf("\n Saldo..........: $%.2f",e.saldo);
						break;
						case 2:
							printf("\n Fecha..........: %02d/%02d/%02d Hora: %02d:%02d:%02d",e.corriente.movi.fec_transaccion.dd,e.corriente.movi.fec_transaccion.mm,e.corriente.movi.fec_transaccion.aaa,e.corriente.movi.fec_transaccion.h,e.corriente.movi.fec_transaccion.m,e.corriente.movi.fec_transaccion.s);
							printf("\n RETIRO.........: $%.2f",e.corriente.movi.ret.valor);
							printf("\n Saldo..........: $%.2f",e.saldo);
						break;
						case 3:
							printf("\n Fecha..........: %02d/%02d/%02d Hora: %02d:%02d:%02d",e.ahorro.movi.fec_transaccion.dd,e.ahorro.movi.fec_transaccion.mm,e.ahorro.movi.fec_transaccion.aaa,e.ahorro.movi.fec_transaccion.h,e.ahorro.movi.fec_transaccion.m,e.ahorro.movi.fec_transaccion.s);
							printf("\n TRANSFERENCIA..: $%.2f",e.corriente.movi.tran.valor);
							printf("\n Saldo..........: $%.2f",e.saldo);
						break;
					}
				break;
			}
			printf("\n________________________________");
			logica=true;
		}
		fread(&e,sizeof(e),1,fich);
	}
	fclose(fich);
}
	if(logica!=true){	printf("\n\tNo existe! o esta INACTIVA");}else{logica=false;	}
getch();
}

void presentar(){
	FILE *fich;DATO e;
	printf("\n\t\t * ULTIMOS MOVIMIENTOS. *\n");
	if ((fich = fopen("OPERACION.dat", "r")) == NULL)  { 
		printf("Fichero no existe\n " );
	} else {
	printf("\n\n\t LISTA DE FICHERO\n");
	fread(&e,sizeof(e),1,fich);
	while(! feof(fich)){
		printf("\n________________________________");
			printf("\n\t No. Transaccion: %i",e.num_transaccion);	
			printf("\n No. Cta........: %i",e.num_cuenta);
			printf("\n Cliente........: %s %s",e.nombre,e.apellido);
			switch(e.tipo_cuenta){
				case 1:
					switch(e.ahorro.movi.tipo){
						case 1:
							printf("\n Fecha..........: %02d/%02d/%02d Hora: %02d:%02d:%02d",e.ahorro.movi.fec_transaccion.dd,e.ahorro.movi.fec_transaccion.mm,e.ahorro.movi.fec_transaccion.aaa,e.ahorro.movi.fec_transaccion.h,e.ahorro.movi.fec_transaccion.m,e.ahorro.movi.fec_transaccion.s);
							printf("\n DEPOSITO.......: $%.2f",e.ahorro.movi.dep.valor);
							printf("\n Saldo..........: $%.2f",e.saldo);
						break;
						case 2:
							printf("\n Fecha..........: %02d/%02d/%02d Hora: %02d:%02d:%02d",e.ahorro.movi.fec_transaccion.dd,e.ahorro.movi.fec_transaccion.mm,e.ahorro.movi.fec_transaccion.aaa,e.ahorro.movi.fec_transaccion.h,e.ahorro.movi.fec_transaccion.m,e.ahorro.movi.fec_transaccion.s);
							printf("\n RETIRO.........: $%.2f",e.ahorro.movi.ret.valor);
							printf("\n Saldo..........: $%.2f",e.saldo);
						break;
						case 3:
							printf("\n Fecha..........: %02d/%02d/%02d Hora: %02d:%02d:%02d",e.ahorro.movi.fec_transaccion.dd,e.ahorro.movi.fec_transaccion.mm,e.ahorro.movi.fec_transaccion.aaa,e.ahorro.movi.fec_transaccion.h,e.ahorro.movi.fec_transaccion.m,e.ahorro.movi.fec_transaccion.s);
							printf("\n TRANSFERENCIA..: $%.2f",e.ahorro.movi.tran.valor);
							printf("\n Saldo..........: $%.2f",e.saldo);
						break;
					}
				break;
				case 2:
					switch(e.corriente.movi.tipo){
						case 1:
							printf("\n Fecha..........: %02d/%02d/%02d Hora: %02d:%02d:%02d",e.corriente.movi.fec_transaccion.dd,e.corriente.movi.fec_transaccion.mm,e.corriente.movi.fec_transaccion.aaa,e.corriente.movi.fec_transaccion.h,e.corriente.movi.fec_transaccion.m,e.corriente.movi.fec_transaccion.s);
							printf("\n DEPOSITO.......: $%.2f",e.corriente.movi.dep.valor);
							printf("\n Saldo..........: $%.2f",e.saldo);
						break;
						case 2:
							printf("\n Fecha..........: %02d/%02d/%02d Hora: %02d:%02d:%02d",e.corriente.movi.fec_transaccion.dd,e.corriente.movi.fec_transaccion.mm,e.corriente.movi.fec_transaccion.aaa,e.corriente.movi.fec_transaccion.h,e.corriente.movi.fec_transaccion.m,e.corriente.movi.fec_transaccion.s);
							printf("\n RETIRO.........: $%.2f",e.corriente.movi.ret.valor);
							printf("\n Saldo..........: $%.2f",e.saldo);
						break;
						case 3:
							printf("\n Fecha..........: %02d/%02d/%02d Hora: %02d:%02d:%02d",e.ahorro.movi.fec_transaccion.dd,e.ahorro.movi.fec_transaccion.mm,e.ahorro.movi.fec_transaccion.aaa,e.ahorro.movi.fec_transaccion.h,e.ahorro.movi.fec_transaccion.m,e.ahorro.movi.fec_transaccion.s);
							printf("\n TRANSFERENCIA..: $%.2f",e.corriente.movi.tran.valor);
							printf("\n Saldo..........: $%.2f",e.saldo);
						break;
					}
				break;
			}
			printf("\n________________________________");
		fread(&e,sizeof(e),1,fich);
	}
}
	fclose(fich);
	getch();
}

void saldos(){
	FILE *fich;DATO e;int num;
	printf("\n\t\t * SALDO DE CLIENTE. *\n");
	if ((fich = fopen("ADMINISTRACION.dat", "r")) == NULL)  { 
		printf("Fichero no existe\n " );
	} else {
	do{
		printf("\n\n Ingrese No. Cta. : ");fflush(stdin);
	}while(!scanf("%i",&num));
	fread(&e,sizeof(e),1,fich);
	while(! feof(fich)){
		if(num==e.num_cuenta){
			printf("\n__________________________");
			printf("\n Cliente..........: %s %s",e.nombre,e.apellido);
			printf("\n Saldo Disponible.: $%.2f",e.saldo);
			printf("\n__________________________");
		}
		fread(&e,sizeof(e),1,fich);
	}
}
	fclose(fich);
	getch();
}

void salir(){
	system("cls"); printf("\n\t*Integrantes:\n \n\t * Ascencio Jose\n\t * Constante Leslie \n\t * Guala Fabricio");getch();
}
