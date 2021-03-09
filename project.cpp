#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<locale.h>
#include <errno.h>
#include<time.h>
//kendime not: Menü üzerinde çalış karakter ekle gibi işlemlerden sonra system cls kullanılabilir ya da başka bir çözüm bulnabilir
//program visual studio ile yazılmaktadır başka platformlarda kodun çalışması için ufak değişiklikler gerekebilir
//not:
//Pokemon oyunu local database yönetici arayüzü
/*Program ana oyunda kullanılan verilerin yönetimi için geliştirilmiştir. Kullanılan veri çeşitliliği ve sayısı normalden fazla olduğu için ve yönetimi
* göreceli daha kolay olduğu için tek yönlü linked list kullanmayı uygun gördüm. Programın ana çalışma mantığı şudur:
* Eğer databasede herhangi bir veri yoksa bir root oluşturuyorum gerekli verileri alıyorum
* ve bunu database'e yazıyorum. Eğer databasede halihazırda veriler varsa da bu verileri bir buffer'a çekip buffer üzerinde istenilen
* işlemi yapıp (basic linked list fonksiyonları v.s) tekrar dosyaya yazıyorum. Bunun dışında listeye rahat erişmek için program arkaplanda
* düğümlere sıralı bir şekilde karakter numaraları veriyorum (karakter silme güncelleme v.s gibi işlemler bu karakter numaraları üzerinden yapılıyor) ve
* karakter numaraları her işlemden sonra otomatik güncelliyorum.
*/

struct n {
	int karakterno;
	char ad[15];
	int hp;
	int attack;
	int defense;
	int sp_atk;
	int sp_def;
	int speed;
	n* next;
};
typedef n node;
//dosya pointer'ı
FILE* fp;
//ana root
node* root = NULL;
//karakter no

//buffer linked list
node* buffer = NULL;

//classic linked list fonksiyonları
node* ekle(node* r);
node* sil(node* r, int x);
void yaz(node* r);
node* guncelle(node* r, int x);
//dosya fonksiyonları
node* dosyadanoku(node* buffe);//dosya güncelleme
node* dosyayaekle(node* r);//tampondaki verileri dosyaya atma
int dosyakontrol(FILE* f);//dosyanın boşluk doluluk bilgisini alan fonksiyon
//ek fonksiyonlar
node* nodefree(node* r);
//int safescanfforint();
void delay(float number_of_seconds);

int main() {
	setlocale(LC_ALL, "");
	printf("Karakter güncelleme arayüzüne hoş geldiniz...\n\n");
	int dosyakont;//dosyanın boşluk doluluk değerini tutan değişken
	int a = 0;//switch içindeki ekstra menülerin tekrarlanması için değişken

	while (1)
	{
		printf("---------------------------------------------\n");
		printf("1.Yeni karakter ekleme arayüzü...\n");
		printf("2.Karakterlerden birini silme arayüzü...\n");
		printf("3.Güncel karakter listesi...\n");
		printf("4.Karakterlerden birinin bilgilerini güncelleme arayüzü..\n");
		printf("5.Programı kapamak için 5'e basınız...\n");

		printf("---------------------------------------------\n");

		int secim;
		scanf_s("%d", &secim);
		switch (secim)
		{
		case 1://karakter ekleme arayüzü kodları
			system("CLS");
			printf("Karakter ekleme ekranına hoşgeldiniz...\n");
		a:
			if ((fp = fopen("deneme.txt", "r")) == NULL) { printf("Dosya veri okumak üzere açılamıyor..."); exit(1); }//dosyada karakter olup olmadığını kontrol ediyor
			dosyakont = dosyakontrol(fp);
			fclose(fp);
			if (dosyakont == 0) { //dosya boşsa ekleme yapıyor.
				printf("\nDosyaya ilk veri ekleniyor\n\n");
				root = ekle(root);
				root = dosyayaekle(root);
			}
			else
			{
				//dosyadan değerleri buffer'a alıp bufferdaki liste üzerinde oynama yapıp yeni listeyi tekrar dosyaya yazıyor
				printf("\ngüncel liste aşağıdaki gibidir..\n");
				buffer = dosyadanoku(buffer);
				yaz(buffer);
				printf("\n");
				buffer = ekle(buffer);
				buffer = dosyayaekle(buffer);
			}
			printf("\nİşlemi tekrarlamak ister misiniz ? \nisterseniz 1'e istemezseniz 2'ye basınız\n");
			scanf_s("%d", &a);
			if (a == 1)
				goto a;
			system("CLS");
			break;

		case 2://karakter silme arayüzü kodları
			system("CLS");
			printf("Karakter silme ekranına hoş geldiniz...\n");
		b:
			if ((fp = fopen("deneme.txt", "r")) == NULL) { printf("Dosya veri okumak üzere açılamıyor..."); exit(1); }//dosyada karakter olup olmadığını kontrol ediyor
			dosyakont = dosyakontrol(fp);
			fclose(fp);
			if (dosyakont == 0) { printf("Dosyada silinecek herhangi bir karakter yok. Karakter ekleyin"); }
			else//dosya boş değilse arzulanan işlem yapılıyor
			{
				//dosyadan değerleri buffer'a alıp bufferdaki liste üzerinde oynama yapıp yeni listeyi tekrar dosyaya yazıyor
				buffer = dosyadanoku(buffer);
				printf("\ngüncel liste aşağıdaki gibidir..\n");
				yaz(buffer);
				printf("\n");
				printf("Silmek istediğiniz karakterin numarasını giriniz...\n");
				int b;
				scanf_s("%d", &b);
				buffer = sil(buffer, b);
				buffer = dosyayaekle(buffer);
			}
			printf("\n\nİşlemi tekrarlamak ister misiniz ? \nisterseniz 1'e istemezseniz 2'ye basınız\n");
			scanf_s("%d", &a);
			if (a == 1)
			{
				system("CLS");
				goto b;
			}
			system("CLS");
			break;

		case 3://karakter listesini ekrana yazdırma kodları
			system("CLS");
			printf("Güncel liste ekranına hoş geldiniz...\n\n");
			if ((fp = fopen("deneme.txt", "r")) == NULL) { printf("Dosya veri okumak üzere açılamıyor..."); exit(1); }//dosyada karakter olup olmadığını kontrol ediyor
			dosyakont = dosyakontrol(fp);
			fclose(fp);
			if (dosyakont == 0) { printf("\nDosyada henüz okunacak bir değer yok...\n"); }
			else //dosya boş değilse arzulanan işlem yapılıyor
			{
				//dosyadan değerleri buffer'a alıp bufferdaki liste üzerinde oynama yapıp yeni listeyi tekrar dosyaya yazıyor
				printf("Güncel liste aşağıdaki gibidir\n\n");
				buffer = dosyadanoku(buffer);
				yaz(buffer);
				buffer = nodefree(buffer);
			}
			printf("\nListeyi bir süre sonra otomatik olarak kapanacaktır...");
			delay(4);
			system("CLS");
			break;

		case 4://karakter değerlerini güncelleme arayüzü
		m:
			system("CLS");
			printf("Karakter güncelleme ekranına hoşgeldiniz...\n\n");
			if ((fp = fopen("deneme.txt", "r")) == NULL) { printf("Dosya veri okumak üzere açılamıyor..."); exit(1); }//dosyada karakter olup olmadığını kontrol ediyor
			dosyakont = dosyakontrol(fp);
			fclose(fp);
			if (dosyakont == 0) { printf("\nDosyada henüz güncellenecek bir değer yok...\n"); }
			//dosya boş değilse arzulanan işlem yapılıyor
			else {
				//dosyadan değerleri buffer'a alıp bufferdaki liste üzerinde oynama yapıp yeni listeyi tekrar dosyaya yazıyor
				buffer = dosyadanoku(buffer);
				int h;
				printf("Güncel liste aşağıdaki gibidir...\n\n");
				yaz(buffer);
				printf("\n\nGüncellemek istediğiniz karakterin numarasını giriniz...\n");
				scanf_s("%d", &h);
				guncelle(buffer, h);
				buffer = dosyayaekle(buffer);
			}
			printf("\n\nİşlemi tekrarlamak ister misiniz ? \nisterseniz 1'e istemezseniz 2'ye basınız\n");
			scanf_s("%d", &a);
			if (a == 1)
			{
				system("CLS");
				goto m;
			}
			system("CLS");
			break;

		case 5:
			printf("\n\nProgram kapatılıyor...");
			return 0;
			break;

		default:
			printf("\n\nHatalı veri girişi yaptınız tekrar deneyiniz...\n\n");
			printf("\nTekrar giriş yapmabilmek için bir süre bekleyiniz...");
			delay(1.5);
			system("CLS");
			break;
		}
	}
}

//Classic node functions
node* ekle(node* r) {
	node* tempu = NULL;
	tempu = r;
	int xu = 1;

	while (tempu != NULL) {
		xu++;
		tempu = tempu->next;
	}

	if (r == NULL) {
		r = (node*)malloc(sizeof(node));
		printf("Karakter bilgilerini sırayla giriniz...\n\n");
		printf("Ad:");
		scanf(" %15s[^\n]", &r->ad);
		printf("\nHp:");
		scanf_s("%d", &r->hp);
		printf("\nAttack:");
		scanf_s("%d", &r->attack);
		printf("\nDefense:");
		scanf_s("%d", &r->defense);
		printf("\nSpecial attack:");
		scanf_s("%d", &r->sp_atk);
		printf("\nSpecial defense:");
		scanf_s("%d", &r->sp_def);
		printf("\nSpeed:");
		scanf_s("%d", &r->speed);
		r->karakterno = xu;
		r->next = NULL;
		return r;
	}
	node* iter;
	iter = r;
	while (iter->next != NULL) { iter = iter->next; }
	iter->next = (node*)malloc(sizeof(node));
	printf("Karakter bilgilerini sırayla giriniz...\n\n");
	printf("Ad:");

	scanf(" %15s[^\n]", &iter->next->ad);
	printf("\nHp:");
	scanf_s("%d", &iter->next->hp);
	printf("\nAttack:");
	scanf_s("%d", &iter->next->attack);
	printf("\nDefense:");
	scanf_s("%d", &iter->next->defense);
	printf("\nSpecial attack:");
	scanf_s("%d", &iter->next->sp_atk);
	printf("\nSpecial defense:");
	scanf_s("%d", &iter->next->sp_def);
	printf("\nSpeed:");
	scanf_s("%d", &iter->next->speed);



	iter->next->karakterno = xu;
	iter->next->next = NULL;
	return r;

}
node* sil(node* r, int x) {

	if (r->karakterno == x) {
		node* temp = r;
		r = r->next;
		free(temp);
		node* temp2 = r;
		while (temp2 != NULL) {
			temp2->karakterno = (temp2->karakterno - 1);
			temp2 = temp2->next;
		}
		return r;
	}
	node* iter;
	iter = r;
	while (iter->next->karakterno != x && iter->next != NULL) { iter = iter->next; }
	if (iter->next == NULL) { printf("Aranan değer düğümde bulunamadı"); return r; }
	node* temp = iter->next;
	temp = iter->next;
	iter->next = iter->next->next;
	free(temp);
	if (iter->next != NULL) {
		while (iter->next != NULL) {
			iter->next->karakterno = iter->next->karakterno - 1;
			iter = iter->next;
		}
	}
	return r;
}
//üzerinde çalış daha iyi olabilir..
void yaz(node* r) {
	node* iter = r;
	if ((fp = fopen("deneme.txt", "r")) == NULL) { printf("Dosya veri okumak üzere açılamıyor...\n"); exit(1); }
	int dosyakont2;
	dosyakont2 = dosyakontrol(fp);
	fclose(fp);
	if (dosyakont2 == 0) { printf("\nDosyada okunacak bir değer yok...\n"); return; }

	if (iter->next == NULL) {
		printf("%d.karakter\nAd=%s\nHP=%d\nAttack=%d\nDefense=%d\nSpecial attack=%d\nSpecial defense=%d\nSpeed=%d\n\n", iter->karakterno, iter->ad, iter->hp, iter->attack, iter->defense, iter->sp_atk, iter->sp_def, iter->speed); return;
	}
	else {
		while (iter != NULL) {
			printf("%d.karakter\nAd=%s\nHP=%d\nAttack=%d\nDefense=%d\nSpecial attack=%d\nSpecial defense=%d\nSpeed=%d\n\n", iter->karakterno, iter->ad, iter->hp, iter->attack, iter->defense, iter->sp_atk, iter->sp_def, iter->speed);
			iter = iter->next;
		}
	}



}
node* guncelle(node* r, int x) {
	node* iter = r;
	while (iter->karakterno != x && iter != NULL) { iter = iter->next; }
	if (iter == NULL) { printf("Güncellemek istediğiniz karakter veri tabanında bulunmuyor...\n\n"); return r; }
	//SYSTEM CLS İLE DAHA GÜZEL GÖRÜNTÜ SAĞLA
	printf("\nKarakterin hangi değerini güncellemek istersiniz ? Menü seçenekleri aşağıdaki gibidir\n\n");
	printf("1.AD\n");
	printf("2.HP\n");
	printf("3.ATTACK\n");
	printf("4.DEFENSE\n");
	printf("5.SPECİAL ATTACK\n");
	printf("6.SPECİAL DEFENSE\n");
	printf("7.SPEED\n");
	printf("8.Tüm bilgileri sırayla güncelle\n");

	int guncelle_menu_secim = 0;
	printf("\nSeçiminizi giriniz:");
	scanf_s("%d", &guncelle_menu_secim);

	switch (guncelle_menu_secim)
	{
	case 1:
		printf("\nYeni adı giriniz:");
		scanf(" %15[^\n]", r->ad);

		break;
	case 2:
		printf("\nYeni hp bilgisini giriniz\n");
		scanf_s("%d", &iter->hp);
		break;
	case 3:
		printf("\nYeni attack bilgisini giriniz\n");
		scanf_s("%d", &iter->attack);
		break;
	case 4:
		printf("\nYeni defense bilgisini giriniz\n");
		scanf_s("%d", &iter->defense);
		break;
	case 5:
		printf("\nYeni special attack bilgisini giriniz\n");
		scanf_s("%d", &iter->sp_atk);
		break;
	case 6:
		printf("\nYeni special defense bilgisini giriniz\n");
		scanf_s("%d", &iter->sp_def);
		break;
	case 7:
		printf("\nYeni speed bilgisini giriniz\n");
		scanf_s("%d", &iter->speed);
		break;
	case 8:
		printf("\nYeni adı giriniz:");
		scanf(" %15s[^\n]", iter->ad);

		printf("\nYeni hp bilgisini giriniz");
		scanf_s("%d", &iter->hp);
		printf("\nYeni attack bilgisini giriniz");
		scanf_s("%d", &iter->attack);
		printf("\nYeni defense bilgisini giriniz");
		scanf_s("%d", &iter->defense);
		printf("\nYeni special attack bilgisini giriniz");
		scanf_s("%d", &iter->sp_atk);
		printf("\nYeni special defense bilgisini giriniz");
		scanf_s("%d", &iter->sp_def);
		printf("\nYeni speed bilgisini giriniz");
		scanf_s("%d", &iter->speed);
		break;
	default:
		printf("\n\nHatalı veri girişi yaptınız tekrar deneyiniz...\n\n");
		break;
	}

	return r;

}
//dosya fonksiyonları
node* dosyayaekle(node* r) {

	if ((fp = fopen("deneme.txt", "w")) == NULL) { printf("Dosya veri yazmak üzere açılamıyor..."); exit(1); }
	node* iter = r;
	while (iter != NULL) {
		//fwrite(&iter, sizeof(node), 1, fp);
		fprintf(fp, "%d\n%s\n%d\n%d\n%d\n%d\n%d\n%d\n", iter->karakterno, iter->ad, iter->hp, iter->attack, iter->defense, iter->sp_atk, iter->sp_def, iter->speed);
		iter = iter->next;
	}
	fclose(fp);
	r = nodefree(r);
	r = NULL;
	return r;
}
node* dosyadanoku(node* buffe) {
	FILE* fp1;
	if ((fp1 = fopen("deneme.txt", "r")) == NULL) { printf("Dosya veri okumak üzere açılamıyor...\n"); exit(1); }

	node* iter = NULL;
	iter = buffe;
	if (iter == NULL) {
		iter = (node*)malloc(sizeof(node));
		buffe = iter;
		buffe->next = NULL;
	}

	while (fscanf(fp1, "%d\n%s\n%d\n%d\n%d\n%d\n%d\n%d\n", &iter->karakterno, &iter->ad, &iter->hp, &iter->attack, &iter->defense, &iter->sp_atk, &iter->sp_def, &iter->speed) != 0)
	{
		if (feof(fp1)) {
			break;
		}
		iter->next = (node*)malloc(sizeof(node));
		iter = iter->next;
		iter->next = NULL;
		//fread
	}

	fclose(fp1);
	return buffe;
}
int dosyakontrol(FILE* f) {
	int a;
	fseek(f, 0L, SEEK_END);
	a = ftell(f);
	rewind(f);
	return a;
}
//ek fonksiyonlar
node* nodefree(node* r) {

	node* store, * p;
	p = r;
	while (p != NULL) {
		store = p->next;
		free(p);
		p = store;
	}
	r = NULL;
	return r;
}
/*   yapım aşamasında bu fonk ile undefined behavior'ı minimuma indirmeyi planlıyorum
int safescanfforint()
{
	long a;
	char buf[16];
	int success;
	int sayac = 0;
	do
	{
		if (sayac != 0) { printf("%d",sayac);  printf("Girilen değer integer bir değer olmalıdır lütfen tekrar deneyiniz...\n"); }

		if (!fgets(buf, 16, stdin))
		{
			return 1;
		}
		char* endptr;
		errno = 0;
		a = strtol(buf, &endptr, 10);
		if (errno == ERANGE)
		{
			printf("Girilen sayı çok büyük ya da çok küçük lütfen tekrar deneyiniz...");
			success = 0;
		}
		else if (endptr == buf)
		{
			printf("Herhangi bir değer girmediniz\n");
			success = 0;
		}
		else if (*endptr && *endptr != '\n')
		{
			success = 0;
		}
		else
		{
			success = 1;
		}
		sayac++;

	} while (!success);

	sayac = 0;

	return a;
}*/

void delay(float number_of_seconds)
{
	float milli_seconds = 1000 * number_of_seconds;
	clock_t start_time = clock();
	while (clock() < start_time + milli_seconds);
}

