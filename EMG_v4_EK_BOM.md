# EMG v4 ek komponent listesi

Bu liste, v2 fixed BOM'a eklenmesi veya v4'te netlestirilmesi gereken
parcalari icerir.

## Yeni / degisen komponentler

| Ref | Deger / Parca | Adet | Not |
|---|---:|---:|---|
| D1 | Low leakage dual ESD diode | 1 | IN+ ve IN- giris korumasi |
| R_INP | 10 kOhm, 1/4W, %1 | 1 | IN+ seri koruma |
| R_INN | 10 kOhm, 1/4W, %1 | 1 | IN- seri koruma |
| C_DIFF | 1 nF, C0G/NP0, 50 V | 1 | IN+ ile IN- arasi RF bastirma |
| C_CM_P | 100 pF, C0G/NP0, 50 V | 1 | Opsiyonel/DNP, IN+ - VREF |
| C_CM_N | 100 pF, C0G/NP0, 50 V | 1 | Opsiyonel/DNP, IN- - VREF |
| R_OUT | 100 Ohm, 1/4W | 1 | RAW_Out seri cikis direnci |
| TP1 | Test point | 1 | IN+ |
| TP2 | Test point | 1 | IN- |
| TP3 | Test point | 1 | VREF |
| TP4 | Test point | 1 | INA_OUT |
| TP5 | Test point | 1 | RAW_Out |
| TP6 | Test point | 1 | GND |
| TP7 | Test point | 1 | VCC |

## Mevcut parcalar icin v4 karari

| Ref | v4 karari |
|---|---|
| U1 INA333 | Kalacak |
| U2 MCP6002 | Kalacak; bos B kanali VREF buffer olarak kullanilacak |
| R2/R3 | Kalacak; orta nokta VREF_RAW olacak |
| C3 | Kalacak; VREF filtreleme icin |
| C4 | 2.2 uF film/polyester, non-polar olacak |
| Cf2 | 330 nF kalacak |
| J1/J2 | Erkek pin header olarak kalabilir; klinik/prototipte kilitli konnektor tercih edilebilir |

## Tedarik notu

ESD diyot seciminde dusuk kacak akim onemli. Genel amacli TVS diyotlar cok
yuksek kacak veya kapasitans yaratirsa EMG girisini bozabilir. Secilecek parca
icin datasheet'te leakage ve capacitance degerleri kontrol edilmeli.
