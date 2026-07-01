# EMG v4 paketi

Bu klasor v2 fixed tasarimindan ayrilmis v4 calisma paketidir.

## Dosyalar

- `EMG_v4.kicad_sch`: v4 adiyla ayrilmis KiCad sema dosyasi.
- `EMG_v4.kicad_pcb`: v4 adiyla ayrilmis PCB dosyasi.
- `EMG_v4_TASARIM_NOTU.md`: v4 icin teknik gelistirme kararlari.
- `EMG_v4_EK_BOM.md`: v4 ile eklenecek komponentler.
- `EMG_v4_VALIDASYON_PROTOKOLU.md`: masa testi ve insan uzerinde tekrar testi.
- `EMG_v4_KONTROL_RAPORU.md`: paketin mevcut durumu ve uretim oncesi kalanlar.

## Kisa karar

v4 icin ana gelistirme: VREF buffer, giris koruma, test noktalari ve net
validasyon proseduru. Bunlar uygulanmadan kartin insan uzerinde guvenilir
olcum verdigini iddia etmek dogru olmaz.

## Uretim notu

Bu paket KiCad calisma paketidir. Eski `gerber` klasoru v2'den kalmistir.
v4 icin Gerber/drill dosyalari yeniden uretilmelidir.
