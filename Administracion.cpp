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
	int num_cuenta;	 //Automática
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

bool logica=false;				// Variable Global
char menu_principal();			// 
char menu_tipo();
int ingresar(int i);
int modificar();
int activar();
int inactivar();
int eliminar();
int consultar();
void salir();
int Val_fecha(int &anio, int &mes);
int validar_num(char cad[50]);
int validar_cad(char cad[50]);
int validar_cuenta(int &e);
int presentar();

int main(void){
	char op,op2;
	do{
		op=menu_principal();
		switch(op){
			case '1':
				do{
					op2=menu_tipo();
					switch(op2){
						case '1':	system("cls");	ingresar(1);	op2='3';	break;
						case '2':	system("cls");	ingresar(2);	op2='3';	break;
					}
				}while(op2!='3');
			break;
			case '2':
				system("cls");
				modificar();
			break;
			case '3':
				system("cls");
				consultar();
			break;
			case '4':
				system("cls");
				activar();
			break;
			case '5':
				system("cls");
				inactivar();
			break;
			case '6':
				system("cls");
				eliminar();
			break;
			case '7':
				system("cls");
				presentar();
			break;
			case '8':
				salir();
			break;
			default:
				printf("\n\n\n\t\t\tOpcion [%c] invalido !",op); getch();
			break;
		}
	}while(op!='8');
	
	return 0;
}
char menu_principal(){
	char op;
	system("cls");
	printf("\n* * Menu Cuentas * *\n");
	printf("\n (1)Crear Cuenta     :");
	printf("\n (2)Modificar Cuenta :");
	printf("\n (3)Consultar Cuenta :");
	printf("\n (4)Activar   Cuenta :");
	printf("\n (5)Inactivar Cuenta :");
	printf("\n (6)Eliminar  Cuenta :");
	printf("\n (7)Todas las Cuenta :");
	printf("\n (8)Salir");
	printf("\n Opcion --> ");fflush(stdin);
	op = getch();
	return op;
}
char menu_tipo(){
	char op;
	system("cls");
	printf("\n* * Menu Tipo * *\n");
	printf("\n (1)Cuenta de Ahorro  :");
	printf("\n (2)Cuenta Corriente  :");
	printf("\n (3)Atras");
	printf("\n Opcion --> ");fflush(stdin);
	op = getch();
	return op;
}
	/* VALIDA INT */
int validar_num(char num[50]){
	int i=0,error=0,j;
	j=strlen(num);
	while(i<j && error==0)
	{
		if(isdigit(num[i])!=0)
		{
			i++;
		}
		else
		{
			error=1;
		}
	}
	return error;
}
/* VALIDA STRING */
int validar_cad(char cad[50]){
	int i=0,error=0,j;
	j=strlen(cad);
	while(i<j && error==0)
	{
		if(isalpha(cad[i]) == 13){ error = 1; return error;}
		if(isalpha(cad[i]) != 0  || cad[i] == 32)
		{
			i++;
		}
		else
		{
			error=1;
		}
	}
	return error;
}
int validar_cuenta(int &e){
	FILE *fich;DATO d;
	if ((fich = fopen("ADMINISTRACION.dat", "r")) == NULL)  { 
		printf("Fichero no existe\n " );
	} else {
	fread(&d,sizeof(d),1,fich);
	while(! feof(fich)){
		if(e == d.num_cuenta){
			while(e == d.num_cuenta){
				e = 9999999+(rand()%99999999); /*----No. Cuenta Automatca----*/
			}
		}
		fread(&d,sizeof(d),1,fich);
	}
	fclose(fich);
}
	return e;
}
int Val_fecha(int &anio, int &mes){
	switch(mes)
	{
		case 1: return 31; break;
		case 2: 
			if (anio%400 == 0 || (anio%4==0 && anio%100!=0)){
            return 29;
            }else{
            return 28;
           	}	
		break;
		case 3: return 31; break;
		case 4: return 30; break;
		case 5: return 31; break;
		case 6: return 30; break;
		case 7: return 31; break;
		case 8: return 31; break;
		case 9: return 30; break;
		case 10: return 31; break;
		case 11: return 30; break;
		case 12: return 31; break;
	}
}
int ingresar(int i){
	FILE *fich; int val_fec;
	DATO e;
	printf("\n\t\t * REGISTRAR cta. *");
	//-----------------------FECHA DEL SISTEMA-------
	time(&t); 
	ahora = localtime(&t);
	//-----------------------------------------------
if ((fich = fopen("ADMINISTRACION.dat", "a")) == NULL)  { 
	printf ("Fichero no existe\n");
}  
else {
	e.num_cuenta = 9999999+(rand()%99999999); /*----No. Cuenta Automatca----*/
	//----------------------------------------------------------
		e.num_cuenta=validar_cuenta(e.num_cuenta);
	//----------------------------------------------------------
	printf("\nCuenta N. %i \n",e.num_cuenta);fflush(stdin);
	do{
		printf("Ingrese CI : ");	fflush(stdin);
		scanf("%s",e.cedula);
	}while(validar_num(e.cedula)==1);
	do{
		printf("Ingrese solo Nombre : ");fflush(stdin);
		gets(e.nombre);
	}while(validar_cad(e.nombre)==1);
	do{
		printf("Ingrese  Apellidos : ");	fflush(stdin);
		gets(e.apellido);
	}while(validar_cad(e.apellido)==1);
	printf("\n\tDireccion vivienda: \n");
	do{
		printf("   Calle Principal : ");	fflush(stdin);
		gets(e.dir_vivienda.calleprin);
	}while(validar_cad(e.dir_vivienda.calleprin)==1);
	do{
		printf("   Calle Secundaria: ");	fflush(stdin);
		gets(e.dir_vivienda.callesec);
	}while(validar_cad(e.dir_vivienda.callesec)==1);
	do{
		printf("   N. Calle: ");	fflush(stdin);
		gets(e.dir_vivienda.num);
	}while(validar_num(e.dir_vivienda.num)==1);
	do{
		printf("Telefono: ");	fflush(stdin);
		gets(e.telefono);
	}while(validar_num(e.telefono)==1);
	printf("\n\tFecha Nacimiento \n");
	do{
		printf("   Anio: ");	fflush(stdin);
	}while(!scanf("%i",&e.fec_nacimiento.aaa)||e.fec_nacimiento.aaa<=1900||e.fec_nacimiento.aaa>2016);
	do{
		printf("   Mes: ");	fflush(stdin);
	}while(!scanf("%i",&e.fec_nacimiento.mm)||e.fec_nacimiento.mm<=0||e.fec_nacimiento.mm>12);
	do{
		printf("   Dia: ");	fflush(stdin);
	}while(!scanf("%i",&e.fec_nacimiento.dd)||e.fec_nacimiento.dd<=0||e.fec_nacimiento.dd>Val_fecha(e.fec_nacimiento.aaa,e.fec_nacimiento.mm));
	//-----------------------FECHA DEL SISTEMA--------------
	e.fec_ingreso.dd = ahora->tm_mday;
	e.fec_ingreso.mm = ahora->tm_mon + 1;
	e.fec_ingreso.aaa = ahora->tm_year +1900;
	//------------------------------------------------------
	if(val_fec = (ahora->tm_year +1900)-(e.fec_nacimiento.aaa)<21){
		printf("\n\n\t No tiene la edad requerida para aperturar una cta.\n");
		printf("\n\t La edad minima es de [20 a%cos].. .\n",164);
		e.num_cuenta = 0;
		getch();
		return 0;
	}
	
	do{
		printf("\nAbono Inicial: $ ");	fflush(stdin);
	}while(!scanf("%f",&e.abono_inicial)|| e.abono_inicial<=0);
	e.saldo = e.abono_inicial;
	
	e.Estado = 1;
	e.tipo_cuenta = i;
	
	fwrite(&e, sizeof(e), 1, fich);
  	fclose(fich);
}
return 1;
	getch();
}
int modificar(){
	FILE *fich , *fich2;
	int op;
	DATO m,e;
	printf("\n\t\t * MODIFICAR cta. *\n");
	if ((fich = fopen("ADMINISTRACION.dat", "r")) == NULL ) { 
		printf("Fichero no existe\n " );
		getch();
		return 0;
	} else 
	if ((fich2 = fopen("aux_.dat", "a")) == NULL ) { 
		printf("Fichero no existe\n " );
		fclose(fich);
	} else {
		printf("\n N. de Cuanta que va a modificar: ");
		scanf("%d",&m.num_cuenta);
		fread (&e, sizeof(e), 1, fich);
		while (! feof(fich)) { 
			if(m.num_cuenta==e.num_cuenta){
				if(e.Estado ==1){
			//-------------------------------------------------------------------------------------------------
			printf("\n(1) -> Cedula: %s ",e.cedula);
			printf("\n(2) -> Nombre: %s ",e.nombre);
			printf("\n(3) -> Apellido: %s",e.apellido);
			printf("\n(4) -> Direccion: %s/%s/#%s",e.dir_vivienda.calleprin,e.dir_vivienda.callesec,e.dir_vivienda.num);
			printf("\n(5) -> Telefono: %s",e.telefono);
			printf("\n(6) -> Fecha Nacimiento: %i/%i/%i",e.fec_nacimiento.dd,e.fec_nacimiento.mm,e.fec_nacimiento.aaa);
			printf("\n(7) -> REGRESAR AL MENU\n\n");
	do{
				do{
					printf(" Que campo desea modificar: ");	fflush(stdin);
				}while(!scanf("%d",&op));
			switch(op){
				case 1: 
					do{
					printf("\n Nuevo CI: ");	fflush(stdin);
					gets(e.cedula);
					}while(validar_num(e.cedula)==1);
					op=7;
					logica=true;
				break;
				case 2:
					do{
					printf("\n Nuevo Nombre: ");	fflush(stdin);
					gets(e.nombre);
					}while(validar_cad(e.nombre)==1);
					op=7;
					logica=true;
				break;
				case 3:
					do{
					printf("\n Nuevo Apellidos: ");	fflush(stdin);
					gets(e.apellido);
					}while(validar_cad(e.apellido)==1);
					op=7;
					logica=true;
				break;
				case 4:
					printf("\n Nuevo DIRECCION: \n");
					do{
					printf("\n\tCalle Principal: ");	fflush(stdin);
					gets(e.dir_vivienda.calleprin);
					}while(validar_cad(e.dir_vivienda.calleprin)==1);
					do{
					printf("\n\tCalle Secundaria: ");	fflush(stdin);
					gets(e.dir_vivienda.callesec);
					}while(validar_cad(e.dir_vivienda.callesec)==1);
					do{
					printf("\n\tCalle Numero: ");	fflush(stdin);
					gets(e.dir_vivienda.num);
					}while(validar_num(e.dir_vivienda.num)==1);
					op=7;
					logica=true;
				break;
				case 5:
					do{
					printf("\n Nuevo Telefono: ");	fflush(stdin);
					gets(e.telefono);
					}while(validar_num(e.telefono)==1);
					op=7;
					logica=true;
				break;
				case 6:
					printf("\n Nuevo FECHA DE NACIMIENTO: \n");	
					do{
					printf("Anio: ");	fflush(stdin);
					}while(!scanf("%i",&e.fec_nacimiento.aaa)||e.fec_nacimiento.aaa<=1900||e.fec_nacimiento.aaa>2016);
					do{
					printf("Mes: ");	fflush(stdin);
					}while(!scanf("%i",&e.fec_nacimiento.mm)||e.fec_nacimiento.mm<=0||e.fec_nacimiento.mm>12);
					do{
					printf("Dia: ");	fflush(stdin);
					}while(!scanf("%i",&e.fec_nacimiento.dd)||e.fec_nacimiento.dd<=0||e.fec_nacimiento.dd>Val_fecha(e.fec_nacimiento.aaa,e.fec_nacimiento.mm));
				op=7;
				logica=true;
				break;
				case 7:
					printf("\n\n\t\t No se ah Modificado ningun Campo. . ."); break;
				break;
			}
	}while(op!=7);
		fwrite(&e, sizeof(e), 1, fich2);//-----------------------------------
	}
	}
	else{//-----------------------------------
		fwrite(&e, sizeof(e), 1, fich2);//-----------------------------------
	}//-----------------------------------
	fread (&e, sizeof(e), 1, fich);
	}
		fclose (fich);
		fclose (fich2);
		remove("ADMINISTRACION.dat");//-----------------------------
		rename("aux_.dat","ADMINISTRACION.dat");
	}
if(logica!=true){  getch();	}else{	printf("\n\n\n\t\t Registro Modificado correctamente . ."); logica=false;getch();}
}
int activar(){
	FILE *fich,*fich2;
	DATO e;
	int num;
	printf("\n\t\t * ACTIVAR cta. *\n");
	if ((fich = fopen("ADMINISTRACION.dat", "r")) == NULL)  { 
		printf("Fichero no existe\n " );
		getch();
		return 0;
	} else 
	if ((fich2 = fopen("aux_.dat", "a")) == NULL)  { 
		printf("Fichero no existe\n " );
		fclose(fich);
	} else {
		do{
		printf("\n\n Ingrese No. Cta. : ");fflush(stdin);
		}while(!scanf("%i",&num));
		fread (&e, sizeof(e), 1, fich);
		while (! feof(fich)) { 
			if(num==e.num_cuenta){
				if(e.Estado == 0){
					e.Estado = 1;
					logica=true;
					fwrite(&e, sizeof(e), 1, fich2);//-----------------------------------
				}else{
					fwrite(&e, sizeof(e), 1, fich2);//-----------------------------------
				}
			}else{
				fwrite(&e, sizeof(e), 1, fich2);//-----------------------------------
			}
		fread (&e, sizeof(e), 1, fich);
	}
		fclose (fich);
		fclose (fich2);
		remove("ADMINISTRACION.dat");//-----------------------------
		rename("aux_.dat","ADMINISTRACION.dat");
	}
	if(logica!=true){	printf("\n\tNo existe O esta ACTIVA!");	getch();	}else{	printf("\n\tCta.No [%i] Activada",num);	logica=false;	getch();	}
}
int inactivar(){
	FILE *fich,*fich2;
	DATO e;
	int num;
	printf("\n\t\t * DESACTIVAR cta. *\n");
	if ((fich = fopen("ADMINISTRACION.dat", "r")) == NULL)  { 
		printf("Fichero no existe\n " );
		getch();
		return 0;
	} else 
	if ((fich2 = fopen("aux_.dat", "a")) == NULL)  { 
		printf("Fichero no existe\n " );
		fclose(fich);
	} else {
		do{
		printf("\n\n Ingrese No. Cta. : ");fflush(stdin);
		}while(!scanf("%i",&num));
		fread (&e, sizeof(e), 1, fich);
		while (! feof(fich)) { 
			if(num==e.num_cuenta){
				if(e.Estado == 1){
					e.Estado = 0;
					logica=true;
					fwrite(&e, sizeof(e), 1, fich2);//-----------------------------------
				}else{
					fwrite(&e, sizeof(e), 1, fich2);//-----------------------------------
				}
			}else{
				fwrite(&e, sizeof(e), 1, fich2);//-----------------------------------
			}
		fread (&e, sizeof(e), 1, fich);
	}
		fclose (fich);
		fclose (fich2);
		remove("ADMINISTRACION.dat");//-----------------------------
		rename("aux_.dat","ADMINISTRACION.dat");
	}
	if(logica!=true){	printf("\n\tNo existe O esta INACTIVA!");	getch();	}else{	printf("\n\tCta.No [%i] Desactivada",num);	logica=false;	getch();	}
}
int eliminar(){
	FILE *fich,*fich2;
	DATO e;
	int num;
	printf("\n\t\t * ELIMINAR cta. *\n");
	if ((fich = fopen("ADMINISTRACION.dat", "r")) == NULL)  { 
		printf("Fichero no existe\n " );
		getch();
		return 0;
	} else 
	if ((fich2 = fopen("aux_.dat", "a")) == NULL)  { 
		printf("Fichero no existe\n " );
		fclose(fich);
	} else {
		do{
		printf("\n\n Ingrese No. Cta. : ");fflush(stdin);
		}while(!scanf("%i",&num));
		fread (&e, sizeof(e), 1, fich);
		while (! feof(fich)) { 
			if(num==e.num_cuenta){
				logica=true;
			}else{
				fwrite(&e, sizeof(e), 1, fich2);//-----------------------------------
			}
		fread (&e, sizeof(e), 1, fich);
	}
		fclose (fich);
		fclose (fich2);
		remove("ADMINISTRACION.dat");//-----------------------------
		rename("aux_.dat","ADMINISTRACION.dat");
	}
	if(logica!=true){	printf("\n\tNo existe O esta INACTIVA!");	getch();	}else{	printf("\n\tCta.No [%i] Eliminada",num);	logica=false;	getch();	}
}
int consultar(){
	FILE *fich; DATO e;
	int num;
	printf("\n\t\t * CONSULTAR cta. *\n");
	if ((fich = fopen("ADMINISTRACION.dat", "r")) == NULL)  { 
		printf("Fichero no existe\n " );
		getch();
		return 0;
	} else {
	do{
		printf("\n\n Ingrese No. Cta. : ");fflush(stdin);
	}while(!scanf("%i",&num));
	fread(&e,sizeof(e),1,fich);
	while(! feof(fich)){
		if(num == e.num_cuenta){
			if(e.Estado == 1){
			printf("\n___________________________");
			printf("\n No. Cta.......: %i",e.num_cuenta);
			printf("\n No. CI........: %s",e.cedula);
			printf("\n Cliente.......: %s %s",e.nombre,e.apellido);
			printf("\n Direccion.....: %s/%s/#%s",e.dir_vivienda.calleprin,e.dir_vivienda.callesec,e.dir_vivienda.num);
			printf("\n No. Telefono..: %s",e.telefono);
			printf("\n Fecha Nacimie.: %i/%i/%i",e.fec_nacimiento.dd,e.fec_nacimiento.mm,e.fec_nacimiento.aaa);
			printf("\n Fecha ingreso.: %i/%i/%i",e.fec_ingreso.dd,e.fec_ingreso.mm,e.fec_ingreso.aaa);
			printf("\n Abono inicial.: $%.2f",e.abono_inicial);
			printf("\n Saldo disponi.: $%.2f",e.saldo);
			switch(e.Estado){
				case 0:
					printf("\n Estado........: Inactivo");
				break;
				case 1:
					printf("\n Estado........: Activo");
				break;
			}
			switch(e.tipo_cuenta){
				case 1:
					printf("\n Cta...........: AHORRO");
				break;
				case 2:
					printf("\n Cta...........: CORRIENTE");
				break;
			}
			printf("\n___________________________");
			logica=true;
			}
		}
		fread(&e,sizeof(e),1,fich);
	}
	fclose(fich);
}
	if(logica!=true){	printf("\n\tNo existe! o esta INACTIVA");	getch();	}else{	getch();	logica=false;	}
}

void salir(){
	system("color 2");system("cls");
	printf("\n\n\t\t UNIVERSIDAD DE GUAYAQUIL ");
	printf("\n\t   FAC - CIENCIAS MATEMATICAS Y FISICAS ");
	printf("\n\n\tCARRERA: INGENIERIA EN SISTEMAS COMPUTACIONALES ");
	printf("\n\tCURSO: S2B ");
	printf("\n\n\t\t\t* * INTEGRANTES * *\n \n\t\t\t * Matute Ronny\n\t\t\t * Marin Kevin\n\t\t\t * Vallejo Felix\n\n\n\n");getch();
}

int presentar(){
	FILE *fich;DATO e;
	printf("\n\t* Listado de todas las cuentas tanto activas como Inactivas . *\n\n");
	if ((fich = fopen("ADMINISTRACION.dat", "r")) == NULL)  { 
		printf("Fichero no existe\n " );
		getch();
		return 0;
	} else {
	fread(&e,sizeof(e),1,fich);
	while(! feof(fich)){
			printf("\n__________________________");
			printf("\n No. Cta.......: %i",e.num_cuenta);
			printf("\n Cliente.......: %s %s",e.nombre,e.apellido);
			switch(e.Estado){
				case 0:
					printf("\n Estado........: Inactivo");
				break;
				case 1:
					printf("\n Estado........: Activo");
				break;
			}
			switch(e.tipo_cuenta){
				case 1:
					printf("\n Cta...........: AHORRO");
				break;
				case 2:
					printf("\n Cta...........: CORRIENTE");
				break;
			}
			printf("\n Saldo disponi.: $%.2f",e.saldo);
			printf("\n__________________________");
		fread(&e,sizeof(e),1,fich);
	}
}
	fclose(fich);
	getch();
}


