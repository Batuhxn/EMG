# EMG v4 kontrol raporu

Tarih: 2026-05-09

## Durum

v4 klasoru, `EMG_v2_working_package_FIXED` klasorunden ayrildi. Dosyalar
`EMG_v4.*` olarak yeniden adlandirildi.

## Yapilanlar

- v4 tasarim notu olusturuldu.
- v4 validasyon protokolu olusturuldu.
- v4 ek BOM listesi olusturuldu.
- v4 BOM ve validasyon Excel dosyasi olusturuldu.
- KiCad proje adlari v4 olarak duzenlendi.
- Eski lock dosyalari kopyadan temizlendi.
- KiCad analiz scriptleri v4 kopyasi uzerinde calistirildi.

## Analiz sonucu

| Kontrol | Sonuc |
|---|---:|
| Sema komponent sayisi | 20 |
| Sema net sayisi | 16 |
| PCB footprint sayisi | 20 |
| Cross-analysis error | 0 |
| Cross-analysis warning | 0 |
| KiCad CLI ERC | 1 warning |
| KiCad CLI DRC | 0 violation |
| KiCad CLI unconnected | 0 |
| KiCad CLI schematic parity | 0 issue |

Sema analizinde gorulen ana uyarilar:

- `DS-001`: datasheet klasoru ve tam MPN kapsami yok; bu nedenle datasheet
  seviyesinde "dogrulandi" denemez.
- `SS-001`: BOM MPN kapsami %50 altinda; uretim oncesi pasifler ve koruma
  parcalari icin net MPN secilmeli.
- `EP-AUD`: J1/J2 icin ESD koruma yok; v4'te zaten bu eksikligi kapatmak icin
  giris koruma eklendi.
- `RS-001`: VCC kaynagi semada regulator/batarya semboluyle tanimli degil;
  prototip icin kabul edilebilir, ama ERC icin netlestirilebilir.

KiCad CLI ile alinan ERC uyarisi:

- `lib_symbol_mismatch`: J2 `Conn_01x04_Pin` sembolunun semadaki kopyasi
  KiCad Connector kutuphanesindeki guncel kopya ile birebir ayni degil.
  Bu baglanti/parite hatasi degil; KiCad'de sembolu library'den update ederek
  temizlenebilir.

KiCad CLI ile `gerber_v4` klasorune Gerber ve `EMG_v4.drl` drill dosyasi
uretildi.

## Bilerek uretime hazir sayilmayan kisim

Bu paket v2 fixed tabaninin v4 calisma paketidir. Gerber/drill dosyalari
KiCad CLI ile uretilmistir; fakat `EMG_v4_TASARIM_NOTU.md` icindeki yeni
v4 donanim ekleri henuz KiCad semasina ve PCB layout'una fiziksel olarak
islenmis degildir.

Sebep:

- Yeni v4 komponentleri PCB uzerine otomatik yerlestirilip route edilmedi.
- Uretilen Gerber, mevcut v2 fixed topolojisinin v4 isimli temiz cikti
  paketidir.

## Uretime gondermeden once gereken son isler

1. `EMG_v4.kicad_sch` KiCad'de acilacak.
2. `EMG_v4_TASARIM_NOTU.md` dosyasindaki v4 ekleri semaya uygulanacak.
3. PCB guncellenecek, yeni footprintler yerlestirilecek.
4. DRC/ERC calistirilacak.
5. Gerber ve drill v4 icin yeniden export edilecek.
6. `EMG_v4_VALIDASYON_PROTOKOLU.md` ile test kaydi alinacak.

## Teknik karar

Bu v4 revizyonu icin en kritik gelistirme sirasi:

1. VREF buffer.
2. Giris koruma.
3. Test pointler.
4. Elektrot yerlestirme/validasyon proseduru.

Bu sira korunursa kartin olcum dogrulugu ve hata ayiklama kabiliyeti en cok
iyilesir.
