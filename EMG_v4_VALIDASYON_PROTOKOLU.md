# EMG v4 validasyon protokolu

Tarih: 2026-05-09

Bu protokol, EMG v4 kartinin dogru calistigini gostermek icin kullanilir.
Amaç, kartin sadece sinyal verip vermedigini degil, beklenen frekans bandinda
tekrarlanabilir ve guvenli sekilde olcum alip almadigini kontrol etmektir.

## 0. Guvenlik

- Insan uzerindeki ilk testlerde kart sadece batarya ile beslenecek.
- Prize bagli guc kaynagi, USB baglantili bilgisayar veya toprakli osiloskop
  ayni anda vucuda baglanmayacak.
- Bu kart tıbbi tani cihazi olarak kullanilmayacak.
- Elektrotlar tek kullanımlık Ag/AgCl jel elektrot olmalı.
- Ciltte yara, tahriş veya hassasiyet varsa test yapılmayacak.

## 1. Gorsel kontrol

| Kontrol | Kabul kriteri |
|---|---|
| Lehim koprusu | Yok |
| INA333 yonu | Dogru |
| MCP6002 yonu | Dogru |
| Elektrolitik C3 polaritesi | Dogru |
| C4 tipi | 2.2 uF non-polar film/polyester |
| J1/J2 pinleri | Dogru dizilim |
| Test noktalari | Etiketli ve ulasilabilir |

## 2. Guc testi

Besleme vermeden once multimetre ile:

- VCC-GND kisa devre yok.
- VREF-GND kisa devre yok.
- RAW_Out-GND kisa devre yok.

Besleme verdikten sonra:

| Olcum | 3.3 V besleme | 5 V besleme |
|---|---:|---:|
| VREF | 1.65 V +/- 5% | 2.50 V +/- 5% |
| RAW_Out, giris bosta degilken | VREF civari | VREF civari |
| Akim | Anormal isinma yok | Anormal isinma yok |

## 3. Masa frekans testi

Giris sinyali:

- Diferansiyel sinyal: 1 mVpp hedef
- Offset: VREF etrafinda
- Frekans noktaları: 10 Hz, 20 Hz, 50 Hz, 100 Hz, 250 Hz, 500 Hz, 1 kHz

Beklenen:

- 10 Hz: HPF sebebiyle dusuk cikis
- 20-250 Hz: kullanilabilir genlik
- 500 Hz: LPF etkisi baslamis
- 1 kHz: belirgin azalma

Kaydedilecek alanlar:

| Frekans | Giriş mVpp | RAW_Out mVpp | Kazanc | Not |
|---:|---:|---:|---:|---|
| 10 Hz | | | | |
| 20 Hz | | | | |
| 50 Hz | | | | |
| 100 Hz | | | | |
| 250 Hz | | | | |
| 500 Hz | | | | |
| 1 kHz | | | | |

## 4. Gurultu testi

Test:

- IN+ ve IN- birlikte VREF'e baglanir.
- RAW_Out osiloskop veya ADC ile kaydedilir.
- 50 Hz gurultu ve taban oynama incelenir.

Kabul:

- RAW_Out VREF etrafinda kalmali.
- Besleme degistiginde VREF oransal olarak degismeli.
- 50 Hz baskin ise elektrot/kablo/ortam iyilestirilmeden insan testi yapilmaz.

## 5. Insan uzerinde tekrarlanabilirlik testi

Elektrot yerlesimi:

- IN+ ve IN- ayni kas uzerinde, kas liflerine paralel.
- Merkezden merkeze hedef mesafe: 20 mm.
- Referans elektrot kas olmayan, elektriksel olarak sakin bolgeye.
- Kablolar hareket etmeyecek sekilde bantlanir.

Protokol:

1. 10 saniye dinlenme.
2. 5 saniye hafif kasilma.
3. 10 saniye dinlenme.
4. 5 saniye guclu kasilma.
5. Bu dongu 3 kez tekrarlanir.

Kaydedilecek:

| Tekrar | Dinlenme RMS | Hafif RMS | Guclu RMS | Not |
|---:|---:|---:|---:|---|
| 1 | | | | |
| 2 | | | | |
| 3 | | | | |

Kabul:

- Hafif kasilma RMS'i dinlenmeden belirgin yuksek olmali.
- Guclu kasilma RMS'i hafiften yuksek olmali.
- Tekrarlar arasinda ayni kasilma seviyesi makul yakinlikta olmali.
- Cikis raylara yapisiyorsa toplam kazanc dusurulmeli.

## 6. Referans sensor karsilastirmasi

Mumkunse ayni kas uzerinde ticari bir EMG sensoruyle ayni hareket kaydedilir.

Karsilastirilacak:

- Kasilma anlarinin zamanlamasi.
- RMS trendi.
- Dinlenme-kasilma ayrimi.
- 50 Hz gurultu farki.

Burada birebir genlik esitligi beklenmez; trendin ve zamanlamanin tutarli
olmasi beklenir.

## 7. Sonuc karari

| Karar | Anlam |
|---|---|
| PASS | Masa testi, gurultu testi ve insan uzerinde tekrar testi kabul edildi |
| CONDITIONAL PASS | Calisiyor ama gürültü, kazanc veya elektrot prosedürü iyilestirilmeli |
| FAIL | VREF, filtre, kazanc veya guvenlik kontrolu gecilemedi |

## 8. Validasyon icin minimum dosyalar

- Osiloskop ekran goruntuleri veya ADC CSV kayitlari.
- Frekans testi tablosu.
- Insan testi RMS tablosu.
- Elektrot yerlesim fotografi veya kroki.
- Test tarihi, besleme gerilimi, kullanilan elektrot tipi.
