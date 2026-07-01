# EMG Sensör v2 — Malzeme Listesi (BOM)

> Kart büyük ölçüde THT (delikli montaj) tasarımlıdır.
> Tek SMD bileşen U1 (INA333, VSSOP-8).

---

## Entegreler

| Ref | Değer | Paket | Adet | Nereden Alınır |
|-----|-------|-------|------|----------------|
| U1 | INA333AIDGKR | VSSOP-8 **SMD** | 1 | [direnc.net](https://www.direnc.net/ina333aidgkr-smd-amplifikator-entegresi-vssop8-en) · [hepsiburada.com](https://www.hepsiburada.com/voltaj-ina333aidgkr-vssop-8-smd-amplifikator-entegresi-pm-HBC0000CP0575) |
| U2 | MCP6002-I/P | DIP-8 THT | 1 | [komponentci.net](https://www.komponentci.net/mcp6002-i-p-islemsel-amplifikator-entegresi-dip-8-pmu9149) · [entegredunyasi.com.tr](https://www.entegredunyasi.com.tr/urun/mcp6002-i-p-pdip-8-operational-amplifier-ic) · [elektrodepo.com](https://www.elektrodepo.com/urun/mcp6002) · [ulutaselektronik.com](https://www.ulutaselektronik.com/urun/mcp6002-i-p-pdip-8-operational-amplifier-ic) |

> **U1 UYARI:** INA333 VSSOP-8 çok küçük bir SMD paketidir (0.65mm pin adımı).
> El lehimlemesi için hot air veya ince uçlu havya + lehim pastası gerekir.
> İlk prototipi bir breakout board üzerinde kurmak kolaylık sağlar.

---

## Dirençler — Axial THT (1/4W yeterli)

| Ref | Değer | Adet | Nereden Alınır |
|-----|-------|------|----------------|
| Rg1, R2, R3 | 10 kΩ | 3 | [direnc.net](https://www.direnc.net/10k-direnc) · komponentci.net · herhangi lokal elektronikçi |
| R4, R5 | 2.2 kΩ | 2 | direnc.net · komponentci.net · lokal elektronikçi |
| R6 | 4.7 kΩ | 1 | direnc.net · komponentci.net · lokal elektronikçi |
| Rf1 | 100 kΩ | 1 | direnc.net · komponentci.net |
| Rg_HPF1 | 10 kΩ | 1 | direnc.net · komponentci.net · lokal elektronikçi |
| R7 | 1 MΩ | 1 | direnc.net · komponentci.net · lokal elektronikçi |

> Tüm dirençler çok yaygın değerler, herhangi bir lokal elektronikçide bulunur.

---

## Kondansatörler

| Ref | Değer | Paket | Adet | Nereden Alınır | Not |
|-----|-------|-------|------|----------------|-----|
| C2, C5, C6 | 100 nF | Disk seramik THT, 5mm adım | 3 | direnc.net · lokal elektronikçi | Standart bypass kap |
| C3 | 10 µF | Elektrolitik radial, D5×P2.5mm | 1 | [direnc.net](https://www.direnc.net/10-uf) · lokal elektronikçi | **Polarite önemli** |
| **C4** | **2.2 µF** | Film/polyester THT, **5mm adım** | 1 | direnc.net · komponentci.net | Aşağıdaki notu oku |
| Cf2 | 330 nF | Disk seramik / film THT, 5mm adım | 1 | direnc.net · komponentci.net | RAW_Out alçak geçiren filtre kondansatörü |

> **C4 NOTU:** Devrede iki ucu da yaklaşık VREF (VCC/2) potansiyelinde olduğu için
> **elektrolitik kullanma** — kutuplamalı çalışmaz veya bozulur.
> **Polyester film (MKT) 2.2µF 50V, 5mm adım** kullan.
> [direnc.net üzerinde "2.2uF film"](https://www.direnc.net/2-2uf-capacitor) veya
> Farnell Türkiye'den Vishay/WIMA MKT serisi ara.
> Bulamazsan 2.2µF **MLCC seramik** (X7R, 10V+) da olur ama bacak adımı farklı olabilir.

---

## Konektörler

| Ref | Değer | Paket | Adet | Nereden Alınır |
|-----|-------|-------|------|----------------|
| J1 | Pin Header 1×3 | 2.54mm pitch, dikey THT | 1 | direnc.net · robotistan.com · lokal |
| J2 | Pin Header 1×4 | 2.54mm pitch, dikey THT | 1 | direnc.net · robotistan.com · lokal |

---

## Önerilen Tedarikçiler (Türkiye)

| Site | Güçlü Olduğu Alan | URL |
|------|-------------------|-----|
| **direnc.net** | Genel pasifler, entegreler, geniş stok | https://www.direnc.net |
| **komponentci.net** | Entegreler, opamplar | https://www.komponentci.net |
| **entegredunyasi.com.tr** | Entegreler | https://www.entegredunyasi.com.tr |
| **elektrodepo.com** | Genel elektronik | https://www.elektrodepo.com |
| **ulutaselektronik.com** | Entegreler, pasifler | https://www.ulutaselektronik.com |
| **özdisan.com** | Profesyonel tedarik | https://www.ozdisan.com |
| **farnell (tr.farnell.com)** | Stok garantili, geniş seçim (kargo ücretli) | https://tr.farnell.com |
| **mouser.com.tr** | Tam stok, orijinal, uluslararası (kargo ücretli) | https://www.mouser.com.tr |

---

## Alım Sırası Önerisi

1. **Önce INA333'ü bul** — en zor bulunan, stok durumu değişken. direnc.net veya hepsiburada'ya bak. Yoksa Mouser'dan sipariş ver.
2. **MCP6002-I/P** — birden fazla sitede var, sorun olmaz.
3. **C4 (2.2µF film)** — elektrolitik değil film olmasına dikkat et.
4. **Geri kalan dirençler ve kaplar** — lokal herhangi bir elektronikçiden temin edilebilir.

---

*v2 — 2026-04-21*
