#include <iostream>

int main() {
	int sayi;
    int toplam = 0;

    std::cout << "Bir sayi giriniz: ";
    sayi = 10;
    //std::cin >> sayi;
    if(sayi > 0){
   		std::cout << "1";
    		toplam += 1;
		for (int i = 3; i <= sayi; i += 2) {
        		std::cout << " + " << i;
        		toplam += i;
    		}
    		std::cout << " = " << toplam << std::endl;
	}
	else{
		std::cout << "tek sayı bulunamadı" << std::endl;
	}
    	return 0;
}
