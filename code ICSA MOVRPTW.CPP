#include <iostream.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <fstream.h>

int i,j,k,l,HASIL,x_best;
int b[150][150], rute[150][150][150],rute_memori[150][150][150],
	calon_rute[150][150],calon_rute_memori[150][150],calon_induk[150];
double gagak_terbaik_sementara[150],pop[150][150],pop_baru[150][150],
	memori[150][150];
double data[150][150],jarak_tempuh[150][150],koor[150][150],
	waktu_tempuh[150][150];
char file[50],nama[50],namafile[50];
float ft[100],ft1[100],ft2[100],ft_memori[150],wmax=0.9,wmin=0.2,
	w_iter,fitness_relatif[150],jarak_total[150],jarak_total_memori[150],
   urutanft[150],urutan[150];
float permintaan,waktu_sampai,waktu_datang,waktu_pelanggan,jarak,
	jarak_terpendek;
double r[150],total_memori[150], fitness[150],fitness_kumulatif[150],
	random_induk[150];

ofstream VRPTW;

void random(int jum_gagak,int jum_pel, double pop[150][150]);
void urut(int l, int jum_pel, double pop[150][150], int b[150][150]);
reset(int l,int jum_pel, float ft[150],int rute[150][150][150]);
void evaluasi(int l,int jum_pel,int calon_rute[150][150],
			double waktu_tempuh[150][150],double jarak_tempuh[150][150],
         double kap_ken,double data[150][150],float ft[150],
         int rute[150][150][150],int jum_ken);
float jarak_terbaik(int jum_gagak,float ft_memori[150],float jarak_terpendek,
							int x_best);
x_terbaik(int jum_gagak,float ft_memori[150],float jarak_terpendek, int x_best);
void urutfitness(int jum_gagak);
void tampilbest(int x_best,int jum_pel,int calon_rute[150][150],
				double waktu_tempuh[150][150],double jarak_tempuh[150][150],
            double kap_ken,double data[150][150],float ft_memori[150],
            int rute_memori[150][150][150],int jum_ken);

void main()
{
   int i,j,cek, pilihdata, benar,jum_pel,jum_ken,max_iterasi1,jum_gagak1,fl1;
   float pilihan,jum_gagak,AP,max_iterasi;
   float fl,kap_ken;
   srand(time(NULL));
   cout<<"================================================================="<<endl;
	cout<<"                IMPROVED CROW SEARCH ALGORITHM (ICSA)            "<<endl;
	cout<<"                       UNTUK MENYELESAIKAN                       "<<endl;
	cout<<"MULTI-OBJECTIVE VEHICLE ROUTING PROBLEM WITH TIME WINDOWS(MOVRPTW)"<<endl;
	cout<<"                             OLEH                                "<<endl;
	cout<<"                   CLARYTA PUTRI DEDYANA WATI                    "<<endl;
	cout<<"                         081911233007                            "<<endl;
	cout<<"================================================================="<<endl<<endl;

	//memilih data
	cout<<"\nMasukkan Pilihan (1/2/3): \n";
	cout<<"1. Data 25 Pelanggan\n";
	cout<<"2. Data 50 Pelanggan\n";
	cout<<"3. Data 100 Pelanggan\n";

   ifstream VRPTWICSA;
   ifstream VRPTWICSAjarak;

   //open data format txt
	do
   {
   	cout<<"\nMasukkan Pilihan (1/2/3)       : ";
   	cin>>pilihan;
   	cek = 0;
   	if(cin.fail())
      {
   		cin.clear();
      	cin.ignore(999, '\n');
      	cek = 1;
      	cout<<"Inputan salah, Masukkan Pilihan (1/2/3) !!!\n";
      }
   	else
		{
			pilihdata = pilihan;
         if (pilihan-pilihdata!=0 || pilihan !=1 && pilihan!=2 && pilihan!=3)
         {
         	cout<<"Inputan salah, Masukkan Pilihan (1/2/3) !!!\n";
            cek = 1;
         }
      }
   }while (cek==1);

   cout<<"\n";

	if (pilihdata == 1)
   {
   	VRPTWICSA.open("timekecil.txt");
      jum_pel=25;kap_ken=200;
      VRPTWICSAjarak.open("koorkecil.txt");
      benar=1;
   }
   else if (pilihdata == 2)
   {
   	VRPTWICSA.open("timesedang.txt");
      jum_pel=50;kap_ken=200;
      VRPTWICSAjarak.open("koorsedang.txt");
      benar=2;
   }

   else if (pilihdata == 3)
   {
   	VRPTWICSA.open("timebesar.txt");
      jum_pel=100; kap_ken=200;
      VRPTWICSAjarak.open("koorbesar.txt");
      benar=3;
   }

   if (benar == pilihdata)
   {
		for(i=1;i<=4;i++)
		{
			for(j=1;j<=jum_pel+1;j++)
			{
   			if(pilihdata==1)
      			{VRPTWICSA>>data[i][j];}
        	 	else if(pilihdata==2)
      			{VRPTWICSA>>data[i][j];}
      		else if (pilihdata==3)
      			{VRPTWICSA>>data[i][j];}
			}
		}
   }

   //inisialisasi parameter
	cout<<"------ INPUT PARAMETER ICSA ------"<<endl;
   //Input maksimum iterasi
   do
	{
   	cout<<"\nMaksimum Iterasi (>=1)         : ";
	  	cin>>max_iterasi;
	  	cek=0;
      if (cin.fail())
      {
			cin.clear();
        	cin.ignore(999,'\n');
        	cek = 1;
        	cout<<"Inputan harus bilangan integer (>0), mohon ulangi !!!\n";
		}
      else
      {
			max_iterasi1=max_iterasi;
			if(max_iterasi1-max_iterasi != 0 || max_iterasi<=0)
   		{
   			cout<<"Inputan harus bilangan integer (>0), mohon ulangi !!!\n";
            cek = 1;
   		}
      }
   }while(cek==1);

   //Input Jumlah Gagak
   do
	{
      cout<<"\nJumlah Gagak (>=2)             : ";
      cin>>jum_gagak;
      cek=0;
      if (cin.fail())
      {
      	cin.clear();
         cin.ignore(999,'\n');
         cek = 1;
         cout<<"Inputan harus bilangan integer (>=2), mohon ulangi !!! \n";
      }
      else
      {
   		jum_gagak1=jum_gagak;
			if(jum_gagak1-jum_gagak!=0||jum_gagak<=1)
   		{
   			cout<<"Inputan harus bilangan integer (>=2), mohon ulangi !!!\n";
            cek = 1;
   		}
      }
	}while(cek==1);

   //Input Panjang Terbang
   do
	{
		cout<<"\nPanjang Terbang (>=1)          : ";cin>>fl;
      cek=0;
      if (cin.fail())
      {
      	cin.clear();
         cin.ignore(999,'\n');
         cek = 1;
         cout<<"Inputan harus bilangan integer (>=1), mohon ulangi !!!\n";
      }
      else
      {
   		fl1=fl;
			if(fl1-fl!=0||fl<=0)
   		{
   			cout<<"Inputan harus bilangan integer(>=1), mohon ulangi !!!\n";
            cek = 1;
   		}
      }
   }while(cek==1);

   //Input Probabilitas Kewaspadaan
   do
	{
   	cout<<"\nProbabilitas Kewaspadaan (0-1) : ";
   	cin>>AP;
   	cek=0;
      if (cin.fail())
      {
      	cin.clear();
         cin.ignore(999,'\n');
         cek = 1;
         cout<<"Inputan harus bilangan real (0,1), mohon ulangi !!! \n";
      }
      else
      {
			if(AP<=0||AP>=1)
			{
				cout<<"Inputan harus bilangan real (0,1), mohon ulangi !!! \n";
            cek = 1;
			}
		}
	}while(cek==1);

	cout<<endl;
	cout<<"nama file : ";
	cin>>file;
	cout<<"nomer urut file : ";
	cin>>HASIL;
	cout<<endl;

	//output notepad
	strcat(file, " ke- "); 	   //menambah karakter kedalam variabel
	strcpy(namafile,file); 		//maksudnya adalah mengcopy isi file ke namafile
	itoa(HASIL,nama,10);	//misal diinputkan Hasil=1234, membuat nama berisi 1234
	strcat(namafile,nama);	//menambah karakter nama kedalam variabel namafile
	strcat(namafile,".txt"); //strcat berfungsi untuk menggabungkan 2 buah string
   								//(melakukan penggabungan variabel namafile dan .txt)
	cout<<namafile<<endl<<endl;
	VRPTW.open(namafile);

	VRPTW<<"====================================================================="<<endl;
	VRPTW<<"                   IMPROVED CROW SEARCH ALGORITHM (ICSA)             "<<endl;
	VRPTW<<"                          UNTUK MENYELESAIKAN                        "<<endl;
	VRPTW<<"  MULTI-OBJECTIVE VEHICLE ROUTING PROBLEM WITH TIME WINDOWS(MOVRPTW) "<<endl;
	VRPTW<<"                                OLEH                                 "<<endl;
	VRPTW<<"                       CLARYTA PUTRI DEDYANA WATI                    "<<endl;
	VRPTW<<"                            081911233007                             "<<endl;
	VRPTW<<"====================================================================="<<endl<<endl;

	VRPTW<<"Parameter yang digunakan         "<<endl;
	VRPTW<<"Kapasitas kendaraan            : "<<kap_ken<<endl;
	VRPTW<<"Jumlah Gagak (>=2)             : "<<jum_gagak<<endl;
	VRPTW<<"Panjang terbang (>=1)          : "<<fl<<endl;
	VRPTW<<"Probabilitas Kewaspadaan (0,1) : "<<AP<<endl;
	VRPTW<<"Data Pelanggan : "<<endl;
	for(i=1;i<=4;i++)
	{
		for(j=0;j<=jum_pel+1;j++)
		{
			VRPTW<<"	"<<data[i][j]<<" ";
    	}
		VRPTW<<endl;
	}

	VRPTW<<endl;

	//waktu tempuh dan jarak tempuh
	if(pilihan==1||pilihan==2 || pilihan==3)

	{
	VRPTW<<"Titik Koordinat : "<<endl;
	VRPTW<<"X	"<<"Y	"<<endl;

		for(i=0;i<=jum_pel;i++)
		{
   		for(j=0;j<=1;j++)
      	{
				VRPTWICSAjarak>>koor[i][j];
				VRPTW<<koor[i][j]<<" 	";
      	}
      	VRPTW<<endl;
   	}
	}

	for(i=0;i<=jum_pel;i++)
	{
		for(j=0;j<=jum_pel;j++)
		{
			if(pilihan==1||pilihan==2 || pilihan==3 )
      	{
         	jarak_tempuh[i][j]=sqrt(pow((koor[i][0]-koor[j][0]),2)+
            							pow((koor[i][1]-koor[j][1]),2));
      		waktu_tempuh[i][j]=jarak_tempuh[i][j]/40;
      	}
    	}
	}
	VRPTW<<endl;

   //Menampilkan Tabel Jarak Tempuh
	VRPTW<<"Jarak antar pelanggan dan Jarak depot ke pelanggan : "<<endl;
	for(i=0;i<=jum_pel;i++)
	{
   	for(j=0;j<=jum_pel;j++)
      {
         VRPTW<<jarak_tempuh[i][j]<<"	";
      }
      VRPTW<<endl;
   }
	VRPTW<<endl;

   //Menampilkan Daftar Tabel Waktu Tempuh
	VRPTW<<"Waktu yang dibutuhkan antar pelanggan dan dari depot ke pelanggan : \n";
	for(i=0;i<=jum_pel;i++)
	{
   	for(j=0;j<=jum_pel;j++)
      {
         VRPTW<<waktu_tempuh[i][j]<<" ";
      }
      VRPTW<<endl;
   }

   // Proses iterasi awal
   VRPTW<<endl;
   VRPTW<<"POSISI AWAL = MEMORI AWAL"<<endl;
   //Memanggil fungsi untuk membangkitkan populasi awal
	random(jum_gagak,jum_pel,pop);
	VRPTW<<endl;
   VRPTW<<"EVALUASI FUNGSI TUJUAN POSISI AWAL"<<endl;
   for ( l = 1; l <= jum_gagak ; l++ )
	{
      VRPTW<<"\nGagak["<<l<<"] "<<endl;
      VRPTW<<"Calon Rute Posisi Gagak["<<l<<"] : ";
      urut(l,jum_pel,pop,calon_rute);
      VRPTW<<endl;
		reset(l,jum_pel,jarak_total,rute);
		evaluasi(l,jum_pel,calon_rute,waktu_tempuh,jarak_tempuh,kap_ken,data,
      			ft,rute,jum_ken);
   }
   VRPTW<<endl;
   VRPTW<<"EVALUASI FUNGSI TUJUAN MEMORI AWAL"<<endl;
   for ( l = 1; l <= jum_gagak ; l++ )
	{
      VRPTW<<"\nGagak["<<l<<"] "<<endl;
      VRPTW<<"Calon Rute Memori Gagak["<<l<<"] : ";
      urut(l,jum_pel,memori,calon_rute_memori);
      VRPTW<<endl;
		reset(l,jum_pel,jarak_total_memori,rute_memori);
		evaluasi(l,jum_pel,calon_rute_memori,waktu_tempuh,jarak_tempuh,
      			kap_ken,data,ft_memori,rute,jum_ken);
   }
   VRPTW<<endl;
	jarak_terpendek=jarak_terbaik(jum_gagak,ft_memori,jarak_terpendek,x_best);
	x_best=x_terbaik(jum_gagak,ft_memori,jarak_terpendek,x_best);
   VRPTW<<"ITERASI KE-0"<<endl;
  	//VRPTW<<"\nFungsi Tujuan Terbaik Pada Iterasi-0 : "<<jarak_terpendek<<endl;
	VRPTW<<"\nGagak Terbaik Pada Iterasi-0  : "<<x_best<<endl;
   evaluasi(x_best,jum_pel,calon_rute_memori,waktu_tempuh,jarak_tempuh,
         			kap_ken,data,ft_memori,rute_memori,jum_ken);
	//VRPTW<<"Posisi gagak terbaik Pada iterasi-0"<<endl;
   cout<<"ITERASI KE-0"<<endl;
   cout<<"\nFungsi Tujuan Terbaik Pada iterasi-0 : "<<jarak_terpendek<<endl;
	cout<<"Gagak Terbaik Pada Iterasi-0         : "<<x_best<<endl;
   tampilbest(x_best,jum_pel,calon_rute_memori,waktu_tempuh,jarak_tempuh,
					kap_ken,data,ft_memori,rute_memori,jum_ken);

	//cout<<"Posisi gagak terbaik pada iterasi-0"<<endl;
   for(i=1;i<=jum_pel;i++)
	{
		gagak_terbaik_sementara[i]=pop[x_best][i];
   	//VRPTW<<gagak_terbaik_sementara[i]<<" ";
      //cout<<gagak_terbaik_sementara[i]<<" ";
	}
	VRPTW<<endl;

   //Masuk ke loopingan iterasi
	for(int iterasi=1;iterasi<=max_iterasi;iterasi++)
	{
      cout<<endl;
      cout<<"ITERASI KE-"<<iterasi<<endl;
      VRPTW<<"\nITERASI KE-"<<iterasi<<endl;
      //memanggil fungsi untuk menghitung fitness relatif,kumulatif,dan seleksi roulettee
      VRPTW<<endl;
		urutfitness(jum_gagak);
      VRPTW<<endl;
      VRPTW<<"\n----------------------------"<<endl;
      //memperbarui posisi
   	VRPTW<<"PROSES UPDATE POSISI GAGAK"<<endl;
   	w_iter=wmax-(wmax-wmin)*(iterasi/max_iterasi);
		for(i=1;i<=jum_gagak;i++)
   	{
   		r[i]=rand()/(double)(RAND_MAX);
      	VRPTW<<"----------------------------\n";
      	VRPTW<<"\nRandom["<<i<<"]                  : "<<r[i];
         if(r[i]>=AP)
      	{
         	VRPTW<<"\nPosisi Gagak["<<i<<"]            : ";
         	for(j=1;j<=jum_pel;j++)
         	{
         		pop_baru[i][j]=w_iter*pop[i][j]+r[i]*fl*
               					(memori[calon_induk[i]][j]-pop[i][j]);
            	pop[i][j]=pop_baru[i][j];
            	VRPTW<<pop[i][j]<<" ";
         	}
         	//VRPTW<<endl;
         }
      	else
      	{
         	VRPTW<<"\nPosisi Gagak["<<i<<"]            : ";
         	for(j=1;j<=jum_pel;j++)
         	{
            	pop_baru[i][j]=rand()/(double)(RAND_MAX);
         		pop[i][j]=pop_baru[i][j];
            	VRPTW<<pop[i][j]<<" ";
         	}
         	//VRPTW<<endl;
      	}
         //Memanggil fungsi untuk Evaluasi Fungsi tujuan posisi baru
         VRPTW<<"\nCalon Rute Posisi Gagak["<<i<<"] : ";
			urut(i, jum_pel,pop,calon_rute);
			reset(i,jum_pel,ft,rute);
			evaluasi(i,jum_pel,calon_rute,waktu_tempuh,jarak_tempuh,
         			kap_ken,data,ft,rute,jum_ken);

   		//Proses Update Memori Gagak
         //VRPTW<<"----------------------------"<<endl;
      	if(ft_memori[i]<ft[i])
      	{
         	VRPTW<<"\nStatus: Memori Tidak Update"<<endl;
         	VRPTW<<"\nMemori Gagak["<<i<<"]            : ";
      		for(j=1;j<=jum_pel;j++)
         	{
         		memori[i][j]=memori[i][j];
            	VRPTW<<memori[i][j]<<" ";
         	}
            //VRPTW<<endl;
         	VRPTW<<"\nCalon Rute Memori Gagak["<<i<<"] : ";
				urut(i, jum_pel,memori,calon_rute_memori);
				reset(i,jum_pel,ft_memori,rute_memori);
				evaluasi(i,jum_pel,calon_rute_memori,waktu_tempuh,jarak_tempuh,
            			kap_ken,data,ft_memori,rute_memori,jum_ken);
            VRPTW<<endl;
      	}
      	else
      	{
         	VRPTW<<"\nStatus: Memori Update"<<endl;
         	VRPTW<<"\nMemori Gagak["<<i<<"]            : ";
      		for(j=1;j<=jum_pel;j++)
         	{
         		memori[i][j]=pop[i][j];
            	VRPTW<<memori[i][j]<<" ";
         	}
            //VRPTW<<endl;
         	VRPTW<<"\nCalon Rute Memori Gagak["<<i<<"] : ";
				urut(i, jum_pel,memori,calon_rute_memori);
				reset(i,jum_pel,ft_memori,rute_memori);
				evaluasi(i,jum_pel,calon_rute_memori,waktu_tempuh,jarak_tempuh,
            			kap_ken,data,ft_memori,rute_memori,jum_ken);
            VRPTW<<endl;
      	}
   	}

      //Prosedur menampilkan solusi terbaik

      //Menampilkan Solusi Terbaik pada setiap iterasi
      if(iterasi<=max_iterasi)
		{
      	jarak_terpendek=jarak_terbaik(jum_gagak,ft_memori,jarak_terpendek,x_best);
			x_best=x_terbaik(jum_gagak,ft_memori,jarak_terpendek,x_best);
  			//VRPTW<<"\nFungsi Tujuan Terbaik Pada Iterasi-"<<iterasi<<" : "<<jarak_terpendek<<endl;
         VRPTW<<"----------------------------\n";
			VRPTW<<"\nGagak Terbaik Pada Iterasi-"<<iterasi<<"  : "<<x_best<<endl;
         evaluasi(x_best,jum_pel,calon_rute_memori,waktu_tempuh,jarak_tempuh,
         			kap_ken,data,ft_memori,rute_memori,jum_ken);
         VRPTW<<"\n----------------------------";
      	//VRPTW<<"Posisi gagak terbaik Pada iterasi-"<<iterasi<<endl;
      	cout<<"\nFungsi Tujuan Terbaik Pada iterasi-"<<iterasi<<" : "<<jarak_terpendek<<endl;
			cout<<"Gagak terbaik Pada Iterasi-"<<iterasi<<"         : "<<x_best<<endl;
         tampilbest(x_best,jum_pel,calon_rute_memori,waktu_tempuh,jarak_tempuh,
         				kap_ken,data,ft_memori,rute_memori,jum_ken);
      	//cout<<"Posisi gagak terbaik Pada iterasi-"<<iterasi<<endl;
   		for(i=1;i<=jum_pel;i++)
			{
				gagak_terbaik_sementara[i]=pop[x_best][i];
   			//VRPTW<<gagak_terbaik_sementara[i]<<" ";
            //cout<<gagak_terbaik_sementara[i]<<" ";
			}
			VRPTW<<endl;
		}
      //Menampilkan Solusi Terbaik Terakhir (Kesimpulan)
      if(iterasi==max_iterasi)
   	{
         //cout<<"\nITERASI KE-"<<iterasi<<endl;
      	VRPTW<<"\n====SOLUSI AKHIR TERBAIK====\n"<<endl;
         cout<<"\n\n================SOLUSI AKHIR TERBAIK================"<<endl;
         cout<<"\nFungsi Tujuan Terbaik  : "<<jarak_terpendek<<endl;
			//cout<<"Gagak terbaik 	      : "<<x_best<<endl;
         tampilbest(x_best,jum_pel,calon_rute_memori,waktu_tempuh,jarak_tempuh,
         				kap_ken,data,ft_memori,rute_memori,jum_ken);
         evaluasi(x_best,jum_pel,calon_rute_memori,waktu_tempuh,jarak_tempuh,
         		   kap_ken,data,ft_memori,rute_memori,jum_ken);
   	}
   }
	cout<<"PROSES SELESAI"<<endl;
	getch();
}

//Fungsi Membangkitkan Posisi Awal
void random(int jum_gagak,int jum_pel, double pop[150][150])
{
	for ( l = 1; l <= jum_gagak ; l++ )
	{
      VRPTW<<"Gagak["<<l<<"]: ";
   	for ( i = 1; i <= jum_pel ; i++)
    	{
    		pop[l][i] = rand()/(double)(RAND_MAX);
         memori[l][i]=pop[l][i];
        	VRPTW<< pop[l][i] << " " ;
    	}
    	VRPTW<<endl;
	}
}

//Fungsi Menentukan Urutan Calon Rute
void urut(int l, int jum_pel, double pop[150][150], int b[150][150])
{
    for ( i = 1; i <= jum_pel; i++)
    {
        k = 1;
        for (j = 1; j <= jum_pel; j++)
        {
            if (pop[l][i] > pop[l][j])
            {
                k += 1;
            }
            b[l][i] = k;
        }
        VRPTW <<  " " << b[l][i] ;
    }
}

reset(int l,int jum_pel, float ft[150],int rute[150][150][150])
{
      i=l;
		ft[i]=0;
		for(k=1;k<=jum_pel;k++)
		{
			for(j=1;j<=jum_pel;j++)
			{
         	rute[i][j][k]=0;
      	}
   	}
   return 0;
}

//Fungsi Menghitung Fungsi Tujuan
void evaluasi(int l,int jum_pel,int calon_rute[150][150],double waktu_tempuh[150][150],
			double jarak_tempuh[150][150],double kap_ken,double data[150][150],
         float ft[150],int rute[150][150][150],int jum_ken)
{

	int tambah_rute,s;
	i=l;
	VRPTW<<"\nRute "<<endl;
	tambah_rute=1;
	j=1;
	jarak=0;
	for(k=1;k<=tambah_rute;k++)
	{
		awal:
		if(j<=jum_pel)
		{
			VRPTW<<" 0 ";
		}
		permintaan=0;
		waktu_sampai=0;
		waktu_datang=0;
		waktu_pelanggan=0;
		s=1;
		while(j<=jum_pel)
   	{
       	permintaan=permintaan+data[1][calon_rute[i][j]];
       	waktu_sampai+=waktu_tempuh[calon_rute[i][j-1]][calon_rute[i][j]];
       	if(permintaan <= kap_ken)
       	{
         	if(waktu_sampai < data[3][calon_rute[i][j]])
            {
          	 	waktu_datang=data[3][calon_rute[i][j]];
               waktu_pelanggan=waktu_datang+data[2][calon_rute[i][j]];
               rute[i][s][k]=calon_rute[i][j];
               VRPTW<<rute[i][s][k]<<" ";
               waktu_sampai=waktu_pelanggan;
               jarak_total[i]=jarak+jarak_tempuh[rute[i][s-1][k]][rute[i][s][k]];
               jarak=jarak_total[i];
               s++;
               j++;
            }
            else if(data[3][calon_rute[i][j]] <= waktu_sampai &&
            			waktu_sampai <= data[4][calon_rute[i][j]])
            {
               waktu_datang=waktu_sampai;
               waktu_pelanggan=waktu_datang+data[2][calon_rute[i][j]];
               rute[i][s][k]=calon_rute[i][j];
               VRPTW<<rute[i][s][k]<<" ";
					waktu_sampai=waktu_pelanggan;
               jarak_total[i]=jarak+jarak_tempuh[rute[i][s-1][k]][rute[i][s][k]];
               jarak=jarak_total[i];
               s++;
               j++;
            }
            if(waktu_sampai > data[4][calon_rute[i][j]])
            {
                 tambah_rute++;
                 jarak_total[i]=jarak+jarak_tempuh[rute[i][s-1][k]][rute[i][s][k]];
                 jarak=jarak_total[i];
                 k++;
                 VRPTW<<" 0 ";
                 VRPTW<<endl;
                 goto awal;
            }
       	}
         else
         {
         	tambah_rute++;
            jarak_total[i]=jarak+jarak_tempuh[rute[i][s-1][k]][rute[i][s][k]];
            jarak=jarak_total[i];
            k++;
            VRPTW<<" 0 ";
            VRPTW<<endl;
            goto awal;
         }
   	}
	}   //k
   //total jarak yang ditempuh
   ft1[i]=jarak_total[i];
   //jumlah kendaraan
   ft2[i]=tambah_rute-1;
   ft[i]=0.001*ft1[i]+100*ft2[i];
	VRPTW<<"Total Jarak      : "<<ft1[i]<<endl;
   VRPTW<<"Jumlah Kendaraan : "<<ft2[i]<<endl;
   VRPTW<<"Fungsi Tujuan    : "<<ft[i]<<endl;
}

// Fungsi untuk sorting nilai fitness
void sorting (int jum_gagak)
{
	double temp;
   for (j=1; j<=jum_gagak; j++)
   {
    urutanft[j]=ft_memori[j];
   }
   for (i=1; i<=jum_gagak; i++)
   {
   	int min = i;
      for (j=1; j<=jum_gagak; j++)
      if (urutanft[j]>urutanft[min])
      {
      	min = j;
         temp = urutanft[i];
         urutanft[i] = urutanft[min];
         urutanft[min]= temp;
      }
   }
}

/* Fungsi Untuk Prosedur Perhitungan Nilai Fitness,
	Ranking, Fitness Relatif dan Fitness kumulatif */
void urutfitness(int jum_gagak)
{
   //Perhitungan nilai fitness dan pengurutan
   float totalft=0,totalfitness=0;
   for(int i=1;i<=jum_gagak;i++)
   {
      VRPTW<<"Rank Gagak ["<<i<<"]:";
      int k=1;
      for (int l=1; l<=jum_gagak; l++)
      {
       	if (ft_memori[i]>ft_memori[l])
         {
          k+=1;
         }
         urutan[i]=k;
      }
      VRPTW<<urutan[i]<<endl;
   }
   sorting(jum_gagak);
   for (i=1; i<=jum_gagak; i++)
   {
   	totalft=totalft+urutanft[i];
   }
   VRPTW<<endl;
	VRPTW<<"Total ft = "<<totalft<<endl;
   VRPTW<<endl<<"Fitness\t|"<<"Fitness Relatif\t|"<<"Fitness Kumulatif";
   for(i=1;i<=jum_gagak;i++)
   {
   	fitness[i]=totalft-urutanft[i];
      totalfitness=totalfitness+fitness[i];
   }
   //Prosedur Perhitungan Fitness Relatif dan Kumulatif
   fitness_kumulatif[0]=0;
   for(i=1;i<=jum_gagak;i++)
   {
   	fitness_relatif[i]=fitness[i]/totalfitness;
      fitness_kumulatif[i]=fitness_kumulatif[i-1]+fitness_relatif[i];
      VRPTW<<endl<<fitness[i]<<"\t| "<<fitness_relatif[i]<<"      \t|"
      <<fitness_kumulatif[i];
   }
   VRPTW<<endl<<endl<<""<<"Rand       \t|"<<"Pilih|";

   // Proses Seleksi Roda Roulette
   for(i=1;i<=jum_gagak;i++)
   {
   	random_induk[i]=rand()/(double)(RAND_MAX);
      VRPTW<<endl<<random_induk[i]<<"\t|";
      for(j=1;j<=jum_gagak;j++)
      {
      	if(random_induk[i]<=fitness_kumulatif[j])
         {
				calon_induk[i]=j;
            VRPTW<<calon_induk[i]<<"\t|";
            break;
         }
      }
   }
}

//Menampilkan Fungsi Tujuan Terbaik dari Setiap Iterasi
float jarak_terbaik(int jum_gagak,float ft_memori[150],float jarak_terpendek,
							int x_best)
{
  	jarak_terpendek=ft_memori[1];
	for(i=1;i<=jum_gagak;i++)
	{
    	if(ft_memori[i]<=jarak_terpendek)
      {
         jarak_terpendek=ft_memori[i];
         x_best=i;
      }
	}
	return(jarak_terpendek);
}

// Menampilkan Gagak Terbaik dari setiap iterasi
x_terbaik(int jum_gagak,float ft_memori[150],float jarak_terpendek, int x_best)
{
	jarak_terpendek=ft_memori[1];
	for(i=1;i<=jum_gagak;i++)
	{
    	if(ft_memori[i]<=jarak_terpendek)
      {
         jarak_terpendek=ft_memori[i];
         x_best=i;
      }
	}
	return(x_best);
}

//Menampilkan Rute dan Fungsi Tujuan di CMD
void tampilbest(int x_terbaik,int jum_pel,int calon_rute[150][150],
				double waktu_tempuh[150][150],double jarak_tempuh[150][150],
            double kap_ken,double data[150][150],float ft_memori[150],
            int rute_memori[150][150][150],int jum_ken)
{
	int tambah_rute,s;
	i=x_best;
	cout<<"Rute Terbaik "<<endl;
	tambah_rute=1;
	j=1;
	jarak=0;
	for(k=1;k<=tambah_rute;k++)
	{
		awal:
		if(j<=jum_pel)
		{
			cout<<" 0 ";
		}
		permintaan=0;
		waktu_sampai=0;
		waktu_datang=0;
		waktu_pelanggan=0;
		s=1;
		while(j<=jum_pel)
   	{
       	permintaan=permintaan+data[1][calon_rute[i][j]];
       	waktu_sampai+=waktu_tempuh[calon_rute[i][j-1]][calon_rute[i][j]];
       	if(permintaan <= kap_ken)
       	{
         	if(waktu_sampai < data[3][calon_rute[i][j]])
            {
          	 	waktu_datang=data[3][calon_rute[i][j]];
               waktu_pelanggan=waktu_datang+data[2][calon_rute[i][j]];
               rute[i][s][k]=calon_rute[i][j];
               cout<<rute[i][s][k]<<" ";
               waktu_sampai=waktu_pelanggan;
               jarak_total[i]=jarak+jarak_tempuh[rute[i][s-1][k]][rute[i][s][k]];
               jarak=jarak_total[i];
               s++;
               j++;
            }
            else if(data[3][calon_rute[i][j]] <= waktu_sampai &&
            			waktu_sampai <= data[4][calon_rute[i][j]])
            {
               waktu_datang=waktu_sampai;
               waktu_pelanggan=waktu_datang+data[2][calon_rute[i][j]];
               rute[i][s][k]=calon_rute[i][j];
               cout<<rute[i][s][k]<<" ";
					waktu_sampai=waktu_pelanggan;
               jarak_total[i]=jarak+jarak_tempuh[rute[i][s-1][k]][rute[i][s][k]];
               jarak=jarak_total[i];
               s++;
               j++;
            }
            if(waktu_sampai > data[4][calon_rute[i][j]])
            {
                 tambah_rute++;
                 jarak_total[i]=jarak+jarak_tempuh[rute[i][s-1][k]][rute[i][s][k]];
                 jarak=jarak_total[i];
                 k++;
                 cout<<" 0 ";
                 cout<<endl;
                 goto awal;
            }
       	}
         else
         {
         	tambah_rute++;
            jarak_total[i]=jarak+jarak_tempuh[rute[i][s-1][k]][rute[i][s][k]];
            jarak=jarak_total[i];
            k++;
            cout<<" 0 ";
            cout<<endl;
            goto awal;
         }
   	}
	}   //k
   ft1[i]=jarak_total[i];
   ft2[i]=tambah_rute-1;
   ft[i]=0.001*ft1[i]+100*ft2[i];
	//cout<<"Total Jarak = "<<ft1[i]<<" Jumlah Kendaraan = "<<ft2[i]<<endl;
   //cout<<"Fungsi Tujuan Gagak-"<<i<<": "<<ft[i]<<endl;
}
