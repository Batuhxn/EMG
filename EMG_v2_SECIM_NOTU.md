# EMG v2 secim notu

## 2026-05-09 FIXED kopya notu

Bu klasor, orijinal `EMG_v2_working_package_20260509-144406.zip` paketine dokunmadan
ayri bir duzeltme kopyasi olarak hazirlandi.

Yapilan duzeltmeler:
- PCB uzerindeki yanlis ikinci `Cf2` footprint'i `C2 = 100nF` olarak duzeltildi.
- `C2` padleri semaya uygun sekilde `VREF` ve `GND` netlerine alindi.
- `Cf2 = 330nF` cikis filtresi olarak korundu.
- `BOM.md` semadaki degerlerle esitlendi.
- `KULLANIM_KILAVUZU.md` kazanc, filtre ve BOM degerleriyle esitlendi.

Kalan notlar:
- Gerber dosyalari yeniden uretilmedi; bu ortamda `kicad-cli` bulunmuyor. Fab'a
  gondermeden once KiCad'de bu FIXED proje acilip Gerber/drill dosyalari yeniden
  export edilmeli.
- Cross-analysis'te kalan tek uyarı `IN+` / `IN-` iz uzunlugu farki. EMG icin
  RF hizinda kritik degil, ama simetri ve ortak-mod gurultu icin layout'ta
  iyilestirilebilir.
- Giris ESD / hasta tarafi koruma devresi hala yok.

Secilen en duzgun set: `v2/v2`.

Neden:
- `v2/v2.kicad_sch`: 20 bilesen, 16 net, 80 wire; bos/placeholder degil.
- `v2/v2.kicad_pcb`: 20 footprint, 2 copper layer, routing complete, unrouted net 0.
- `v2/v2 gerber 3`: KiCad 10 ile uretilmis; beklenen Gerber katmanlari ve PTH/NPTH drill dosyalari mevcut.

Paket kapsami:
- KiCad proje dosyalari: `v2.kicad_pro`, `v2.kicad_sch`, `v2.kicad_pcb`, `v2.kicad_prl`
- Uretim ciktilari: `v2 gerber 3` klasoru ve `v2 gerber 3.zip`
- Dokumanlar: `BOM.md`, `KULLANIM_KILAVUZU.md`
- Analiz notlari: bu dosya

Dikkat:
- Analyzer, VCC icin power flag/regulator cikisi uyarisi ve MPN eksikligi verdi. Bunlar kartin route edilmis olmasini bozmaz, ama siparis/onay oncesi KiCad ERC/DRC ile son kontrol onerilir.
- Gerber analizinde paste/alignment uyarilari var; katman seti tam gorunuyor, yine de fab'a gondermeden once Gerber viewer ile hizalama gorsel kontrolu yapilmalidir.
