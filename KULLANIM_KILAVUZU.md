# EMG Sensör Kartı v2 — Kullanım Kılavuzu

## 1. Genel Bakış

Bu kart, kaslardan gelen biyoelektrik sinyali (EMG — Electromyography) yüzey elektrodu
aracılığıyla ölçmek için tasarlanmış iki aşamalı bir analog ön işleme devresidir.

**Sinyal zinciri:**
```
Elektrotlar → INA333 (enstrümantasyon amp.) → Yüksek Geçirgen Filtre → MCP6002 (kazanç + alçak geçirgen filtre) → Çıkış
```

Devrenin amacı 1 mV mertebesindeki zayıf kas sinyalini yükselterek bir mikrodenetleyicinin
ADC'si veya osiloskop ile okunabilir hale getirmektir.

---

## 2. Teknik Özellikler

| Parametre                  | Değer                        |
|---------------------------|------------------------------|
| Besleme gerilimi (VCC)     | 3.3 V veya 5 V               |
| Akım tüketimi (tipik)      | ~1 mA                        |
| Referans gerilimi (VREF)   | VCC / 2 (orta ray)           |
| INA333 kazancı             | 11×  (Rg1 = 10 kΩ)           |
| MCP6002 kazancı            | ~11× (Rf1/Rg1 = 100k/10k)     |
| Toplam kazanç              | ~121×                        |
| Yüksek geçirgen filtre     | ~15 Hz  *(C4 = 2.2 µF ile)*  |
| Alçak geçirgen filtre      | ~482 Hz *(R8 = 1 kΩ, Cf2 = 330 nF)* |
| Efektif bant genişliği     | 15 Hz – 482 Hz               |
| Kart boyutu                | 68.5 mm × 29 mm              |
| Katman sayısı              | 2 katman, 1.6 mm FR4         |

> **Uyarı:** Fabrikadan gelen kartta C4 = 82 nF monte edilmişse yüksek geçirgen filtre
> 413 Hz'de keser ve EMG sinyalinin büyük kısmı geçemez. C4'ü **2.2 µF film/polyester**
> veya uygun non-polar kondansatörle değiştirmeden kart düzgün ölçüm yapmaz.

---

## 3. Konektörler ve Pin Dizilimi

### J1 — Elektrot Girişi (3 pin, 2.54 mm pitch)

```
J1
Pin 1  →  IN+       (Aktif elektrot — ölçülen kasın üzerindeki 1. elektrot)
Pin 2  →  IN-       (Aktif elektrot — ölçülen kasın üzerindeki 2. elektrot)
Pin 3  →  BODY_REF  (Referans elektrot — vücudun nötr noktası)
```

### J2 — Çıkış / Besleme (4 pin, 2.54 mm pitch)

```
J2
Pin 1  →  VCC      (Besleme girişi: 3.3 V veya 5 V)
Pin 2  →  RAW_Out  (İşlenmiş EMG sinyal çıkışı)
Pin 3  →  VREF     (VCC/2 referans — ADC için gerekli)
Pin 4  →  GND      (Toprak)
```

> J2 Pin1 ve Pin4'ten karta besleme sağlanır; Pin2 ve Pin3 ADC'ye bağlanır.

---

## 4. Güç Bağlantısı

1. J2 Pin1'e 3.3 V veya 5 V uygulayın.
2. J2 Pin4'ü GND'ye bağlayın.
3. Besleme kaynağı temiz olmalıdır; adaptör kullanıyorsanız 100 µF bulk kondansatör
   ekleyin. Pille (LiPo, AAA x2 vb.) besleme daha düşük gürültü verir.

---

## 5. Elektrot Bağlantısı

### Gerekli malzeme
- 3 adet yüzey (yüzeysel) EMG elektrodu (yapışkanlı Ag/AgCl jel elektrot önerilir)
- 3 adet elektrot kablosu / snap kablo

### Yerleştirme

```
   IN+  (J1-1) ──►  Elektrot A  ]
                                 }── Ölçülen kasın üzerinde,
   IN-  (J1-2) ──►  Elektrot B  ]   kas lifine paralel, birbirine ~2 cm
   
   BODY_REF (J1-3) ──►  Referans elektrodu  ]── Kemik çıkıntı veya kas olmayan
                                                  bir bölge (dirsek, bilek kemeri vb.)
```

**Altı kural:**
1. Cilt hazırlığı yapın: alkollü mendille elektrodu koyacağınız alanı silin, kurulayın.
2. IN+ ve IN- elektrotlarını kas lifi yönüne **paralel** yerleştirin.
3. İki aktif elektrot arasındaki mesafe 1.5–2.5 cm olmalıdır.
4. Referans elektrodu mümkün olduğunca kasın **uzağına** koyun (elektrik gürültüsü azalır).
5. Kablo hareketi sırasında kopma olmaması için kabloları vücuda bantlayın.
6. Ölçüm sırasında kasın üzerinden elektrot kaldırmayın.

---

## 6. ADC / Osiloskop Bağlantısı

| Sinyal  | J2 Pin | Açıklama                                              |
|---------|--------|-------------------------------------------------------|
| RAW_Out | Pin 2  | EMG çıkışı, VREF etrafında salınır (AC + DC offset)  |
| VREF    | Pin 3  | VCC/2 ≈ 2.5 V (5V'de) veya 1.65 V (3.3V'de)        |
| GND     | Pin 4  | Referans                                              |

### Mikrodenetleyici ADC (ör. Arduino, STM32)

```
J2-Pin2 (RAW_Out) → ADC girişi
J2-Pin3 (VREF)    → ADC referans veya ayrı ölçüm kanalı
J2-Pin4 (GND)     → MCU GND
```

ADC okuma formülü:
```
EMG_mV = (ADC_değer - VREF_ADC_değer) × (VCC_mV / ADC_çözünürlük)
```

Örnek (5 V, 10-bit ADC, VREF = 512 count):
```
EMG_mV = (okunan - 512) × (5000 / 1024)
```

### Osiloskop

- Prob: CH1 → J2 Pin2, GND klibi → J2 Pin4
- Zaman tabanı: 10–50 ms/div
- Gerilim: 100–500 mV/div
- Coupling: AC tercih edilir (DC offset'i bastırır)

---

## 7. Beklenen Çıkış Sinyali

| Kas durumu  | RAW_Out (5V besleme)         |
|-------------|------------------------------|
| Dinlenme    | ~2.5 V (VREF, ±50 mV gürültü) |
| Hafif kasılma | VREF ± 200–500 mV          |
| Güçlü kasılma | VREF ± 500 mV–1 V          |

Toplam kazanç ~121× olduğundan:
- 1 mV EMG girişi → ~121 mV çıkış salınımı
- Yaklaşık 8-10 mV diferansiyel girişte çıkış ray voltajına yaklaşabilir — daha güçlü kasılmalarda kırpma oluşur.

---

## 8. Bilinen Limitasyonlar

| Konu             | Açıklama                                                    |
|------------------|-------------------------------------------------------------|
| Kırpma           | Güçlü kasılmalarda çıkış ray voltajına yapışabilir; Rg1'i artırarak kazancı düşürün |
| Güç hattı gürültüsü | Adaptörle beslemede 50/60 Hz hat gürültüsü görünebilir; pil kullanın |
| Elektrot kalitesi | Kötü temas = yüksek ortak mod gürültüsü; Ag/AgCl jel elektrot kullanın |
| Tek kanal        | Kart tek kas kanalı ölçer; ikinci kas için ikinci kart gerekir |
| İzolasyon yok    | Kart gövde izolasyonu içermez; güvenlik açısından pil ile besleyin |

---

## 9. Sorun Giderme

| Belirti                          | Olası neden                          | Çözüm                                      |
|----------------------------------|--------------------------------------|--------------------------------------------|
| Çıkış hiç salınmıyor             | C4 = 82 nF (413 Hz HPF)             | C4'ü 2.2 µF ile değiştirin                 |
| Çıkış sürekli 0 V veya VCC       | Besleme yok / kırpma                | Beslemeyi ve kazancı kontrol edin           |
| Çok fazla gürültü (50/60 Hz)     | Adaptör beslemesi / kötü elektrot   | Pil kullanın, elektrotu yenileyin           |
| Kasılma yok gibi görünüyor       | Elektrot yeri yanlış                | Kas lifine paralel, 2 cm aralıkla yerleştirin |
| VREF yanlış (VCC/2 değil)        | R2/R3 arızalı veya C3 kısa devre   | R2, R3 = 10 kΩ olduğunu ölçerek teyit edin |
| INA333 ısınıyor                  | Besleme polarity tersine bağlı      | J2 Pin1/Pin4 polaritesini kontrol edin      |

---

## 10. Komponent Listesi (BOM)

| Ref    | Değer      | Açıklama                              |
|--------|------------|---------------------------------------|
| U1     | INA333     | Enstrümantasyon amplifikatörü (VSSOP-8) |
| U2     | MCP6002    | Çift op-amp (DIP-8)                   |
| Rg1    | 10 kΩ      | INA333 kazanç direnci (G = 11×)       |
| R2, R3 | 10 kΩ     | VREF gerilim bölücü                   |
| R4, R5 | 2.2 kΩ    | Giriş seri dirençleri                 |
| R6     | 4.7 kΩ    | HPF bias direnci                      |
| R7     | 1 MΩ       | BODY_REF bias direnci                 |
| Rg_HPF1 | 10 kΩ    | INA333 kazanç direnci                 |
| Rf1    | 100 kΩ    | MCP6002 geri besleme direnci          |
| **C4** | **2.2 µF** | **HPF kondansatörü (kritik: 82nF DEĞİL)** |
| Cf2    | 330 nF     | RAW_Out LPF kondansatörü (~482 Hz)    |
| C2, C5, C6 | 100 nF | Besleme bypass                        |
| C3     | 10 µF      | VREF filtreleme (elektrolitik, radial) |
| J1     | 3-pin 2.54mm | Elektrot girişi                    |
| J2     | 4-pin 2.54mm | Çıkış / besleme                    |

---

*v2 — 2026-04-21*
