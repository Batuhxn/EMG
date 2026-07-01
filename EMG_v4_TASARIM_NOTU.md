# EMG v4 gelistirilmis tasarim notu

Tarih: 2026-05-09

Bu paket, v2 fixed kartinin uzerine daha guvenli ve daha tekrarlanabilir EMG
olcumu almak icin hazirlanan v4 revizyon paketidir.

Onemli durum: Bu klasorde KiCad dosyalari v4 adi ile ayrildi. Ancak bu ortamda
KiCad CLI olmadigi icin yeni PCB yerlesimi, DRC/ERC ve Gerber yeniden uretimi
otomatik yapilmadi. Bu dosyadaki degisiklikler KiCad sema/PCB uzerinde
uygulanip DRC/ERC temizlendikten sonra uretime gonderilmelidir.

## v4 hedefi

- Elektrot temasindan gelen DC offset ve hareket kaynakli gürültüye karsi daha
  kararlı sinyal almak.
- Hasta/elektrot tarafinda temel giris korumasi eklemek.
- VREF hattini dusuk empedansli ve olculebilir yapmak.
- Hata ayiklama icin test noktalarini eklemek.
- Validasyonu sadece "sinyal geliyor" seviyesinde degil, masa testi ve insan
  uzerinde tekrarlanabilirlik testiyle yapmak.

## Literatür ve uygulama dayanaklari

- SENIAM yuzey EMG icin iki aktif elektrot arasinda genelde 20 mm merkezden
  merkeze mesafe ve elektrot hattinin kas liflerine paralel olmasini onerir.
- SENIAM referans elektrodun elektriksel olarak daha sakin bir bolgeye
  yerlestirilmesini onerir.
- Analog Devices biyopotansiyel notunda Ag/AgCl elektrotlarin dusuk
  half-cell potansiyel ve hareket artefaktina karsi daha uygun oldugu, cilt
  hazirliginin sinyal kalitesini ciddi etkiledigi anlatilir.
- TI INA333 datasheet'inde REF pininin dusuk empedansli surulmesi gerektigi ve
  girislerde ray disina cikma durumunda akimin seri direnclerle sinirlanmasi
  gerektigi belirtilir.

Kaynaklar:
- https://seniam.org/fixation.htm
- https://www.analog.com/en/resources/technical-articles/biopotential-electrode-sensors-ecg-eeg-emg.html
- https://www.ti.com/lit/ds/symlink/ina333.pdf
- https://www.law.cornell.edu/cfr/text/21/890.1375
- https://www.vde.com/topics-en/health/consulting/electrical-safety-in-active-medical-devices--the-iec-60601-1-standard

## v4 devre degisiklikleri

### 1. VREF buffer

Mevcut v2'de VREF, R2/R3 bolucu ve kondansatorlerle olusuyor. v4'te MCP6002'nin
bos kalan B kanali VREF buffer olarak kullanilmali.

Onerilen net isimleri:

- R2/R3 orta noktasi: VREF_RAW
- U2B cikisi: VREF
- INA333 REF pini, filtre bias noktalari ve J2 VREF pini: VREF

Baglanti:

- U2B non-inverting input: VREF_RAW
- U2B inverting input: U2B output
- U2B output: VREF
- C2 100 nF: VREF-GND
- C3 10 uF: VREF-GND

Neden: INA333 REF pini dusuk empedans ister. VREF kayarsa tum cikis offseti
kayar.

### 2. Giris koruma

Elektrot girisine temel koruma eklenmeli.

Onerilen ekler:

- D1: dusuk kacakli dual ESD koruma diyodu, IN_PROT+ ve IN_PROT- icin.
- R_INP: 10 kOhm seri giris direnci, elektrot IN+ ile INA333 IN+ arasinda.
- R_INN: 10 kOhm seri giris direnci, elektrot IN- ile INA333 IN- arasinda.
- C_DIFF: 1 nF C0G/NP0, INA333 girisleri arasinda RF bastirma icin.
- C_CM_P ve C_CM_N: 100 pF C0G/NP0, her giristen VREF'e; opsiyonel/DNP.

Not: Mevcut R4/R5 de seri giris elemani gibi kullaniliyor. v4'te bunlarin
fonksiyonu netlestirilmeli; ya R4/R5 10 kOhm'a cekilmeli ya da yeni R_INP/R_INN
eklenip R4/R5 ayni agda gereksiz tekrar olusturmayacak sekilde duzenlenmeli.

### 3. Test noktalari

PCB uzerine asagidaki test noktalarini ekle:

- TP1: IN+
- TP2: IN-
- TP3: VREF
- TP4: INA_OUT
- TP5: RAW_Out
- TP6: GND
- TP7: VCC

Neden: Bring-up sirasinda sorun varsa hangi katta oldugunu 5 dakikada ayirmayi
saglar.

### 4. Cikis korumasi

ADC veya osiloskop baglantisinda cikis kararliligi icin RAW_Out hattina 100 Ohm
seri direnç eklenebilir.

Onerilen:

- R_OUT: 100 Ohm, MCP6002 cikisi ile J2 RAW_Out arasinda.

### 5. Guc ve kullanim siniri

Insan uzerinde ilk testler sadece batarya ile yapilmali. USB, masa tipi guc
kaynagi veya prize bagli osiloskopla vucuda bagli olcum alinmamali. Tıbbi
cihaz iddiası icin IEC 60601 ailesi ve ilgili regülasyonlar gerekir.

## v4 beklenen teknik degerler

| Parametre | Hedef |
|---|---:|
| Besleme | 3.3 V veya 5 V, batarya tercih edilir |
| VREF | VCC/2, bufferli |
| INA333 kazanci | 11x, Rg = 10 kOhm |
| Ikinci kat kazanci | Yaklasik 11x |
| Toplam kazanc | Yaklasik 121x |
| HPF | Yaklasik 15 Hz |
| LPF | Yaklasik 482 Hz |
| Elektrot araligi | 20 mm hedef |
| Elektrot yeri | Kas lifine paralel |

## Uretim oncesi karar

Bu v4 paketi tasarim ve validasyon olarak hazirdir; fab'a gonderme karari icin
su uc adim tamamlanmali:

1. KiCad'de v4 sema degisiklikleri uygulanacak.
2. PCB uzerinde yeni footprintler yerlestirilip DRC/ERC temizlenecek.
3. Gerber ve drill dosyalari v4 icin yeniden export edilecek.
