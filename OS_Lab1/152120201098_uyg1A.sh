#scriptiniz executable olmalıdır. Bu değişiklikleri nasıl sağladığınızı ve nasıl çalıştırılacağını içeren komutları dosyanın en üstüne açıklama satırı olarak ekleyiniz.
!/bin/zsh
chmod +x 152120201098_uyg1A.sh

#Eğer yoksa tempFiles isminde klasör yaratın. Aynı isimde bir klasör varsa ilgili komut hataya veya uyarıya düşmemelidir.
mkdir -p tempFiles

#Çalışma yolundaki tüm içerik; çalışma yolu, alt klasör içerikleri ve (yalnızca bu adım için) boyut detayları ile birlikte ters sırada ve her sırada bir öğe olacak şekilde listelenir. Kodun derlenmesi alt adımına kadar tüm alt adımların hemen ardından basite listeleme (sadece isimler yeterlidir), işlemlerin etkisini gözlemleyebilmek üzere tekrarlanır.
ls -R | sort -r > tempFiles

#<dil> ve .sh uzantılı tüm dosyalar tempFiles klasörüne taşınır.
find . -type f -name "*.sh" -exec mv {} tempFiles/ \;
find . -type f -name "*.cpp" -exec mv {} tempFiles/ \;
#Çalışma yolundaki tüm dosyalar (klasör harici tüm içerik) silinir.
shopt -s extglob
rm -rf !(tempFiles)
#tempFiles klasöründeki tüm içerik çalışma yoluna kopyalanır.
cp -r tempFiles/* .
#tempFiles klasörü (içeriğiyle birlikte) silinir.
rm -rf tempFiles
#kaynak kod derlenir.
g++ 152120201098.uyg1A.cpp -o program
./program > "152120201098_uyg1A.txt"
# Karakter sayısını hesapla
char_count=$(wc -m < "152120201098_uyg1A.txt")
# Kelime sayısını hesapla
word_count=$(wc -w < "152120201098_uyg1A.txt")
# Sonuçları ekrana yazdır
echo "Karakter sayısı: $char_count, Kelime sayısı: $word_count"
#Çıktı dosyasının tüm içeriği ekrana yazdırılır.
cat 152120201098_uyg1A.txt
