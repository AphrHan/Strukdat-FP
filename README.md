1. Pendahuluan
collision detection (pendeteksian tabrakan) merupakan komponen yang sangat penting. Collision detection digunakan untuk menentukan apakah dua atau lebih objek saling bertabrakan berdasarkan posisi dan ukuran masing-masing objek. Ketepatan dan efisiensi collision detection sangat mempengaruhi performa simulasi, terutama ketika jumlah objek yang disimulasikan cukup banyak. Pada kesempatan ini dilakukan simulasi collision detection menggunakan bahasa pemrograman C++ dengan bantuan library SFML. Dua metode yang dibandingkan adalah Brute Force dan Quadtree. Kedua metode tersebut diimplementasikan untuk mendeteksi tabrakan antar bola dalam ruang dua dimensi.

3. Deskripsi Simulasi
Simulasi yang dibuat menampilkan sejumlah bola yang bergerak secara acak di dalam sebuah jendela 2D. Setiap bola memiliki atribut posisi, kecepatan, jari-jari, massa, dan warna. Bola akan memantul ketika mengenai dinding dan akan mengalami perubahan kecepatan ketika bertabrakan dengan bola lain.

Kita dapat:

- Menambahkan bola baru dengan klik mouse
- Mengganti metode collision detection menggunakan tombol Q
- Mengamati perbedaan performa antara metode Brute Force dan Quadtree

3. Metode Brute Force
3.1 Konsep Brute Force

Metode Brute Force melakukan pengecekan tabrakan dengan cara membandingkan setiap objek dengan seluruh objek lainnya. Untuk setiap pasangan objek, jarak antar pusat objek dihitung dan dibandingkan dengan jumlah jari-jari kedua objek.

3.2 Cara Kerja

- Ambil objek ke-i
- Bandingkan dengan objek ke-(i+1) hingga objek terakhir
- Jika jarak antar objek lebih kecil dari jumlah jari-jari, maka terjadi tabrakan

3.3 Kelebihan Brute Force

- Implementasi sangat sederhana
- Mudah dipahami
- Cocok untuk jumlah objek yang sedikit

3.4 Kekurangan Brute Force

- Tidak efisien untuk jumlah objek besar
- Jumlah perhitungan meningkat sangat cepat
- Performa turun drastis pada simulasi real-time

3.5 Notasi Kompleksitas

Kompleksitas waktu metode Brute Force adalah:

O(N²)

Dimana N adalah jumlah objek. Hal ini terjadi karena setiap objek dibandingkan dengan semua objek lainnya.

4. Metode Quadtree
4.1 Konsep Quadtree

Quadtree adalah struktur data berbasis pembagian ruang (spatial partitioning) yang membagi ruang dua dimensi menjadi empat bagian (kuadran). Setiap kuadran hanya menyimpan objek yang berada di dalam wilayah tersebut. Dengan Quadtree, collision detection hanya dilakukan pada objek-objek yang berada di area yang berdekatan, sehingga mengurangi jumlah pengecekan yang tidak perlu.

4.2 Cara Kerja Quadtree

- Ruang simulasi dibagi menjadi empat kuadran
- Objek dimasukkan ke kuadran sesuai posisinya
- Jika satu kuadran terlalu penuh, kuadran tersebut dibagi kembali
- Untuk satu objek, collision hanya dicek dengan objek dalam kuadran yang sama atau berdekatan

4.3 Kelebihan Quadtree

- Jauh lebih efisien untuk objek dalam jumlah besar
- Mengurangi jumlah pengecekan collision
- Cocok untuk simulasi dan game real-time

4.4 Kekurangan Quadtree

- Implementasi lebih kompleks
- Membutuhkan struktur data tambahan
- Perlu pembaruan struktur setiap frame

4.5 Notasi Kompleksitas

Kompleksitas waktu rata-rata Quadtree adalah:

O(N log N)

Dalam kondisi tertentu (objek sangat rapat), kompleksitas bisa mendekati O(N²), namun kasus ini jarang terjadi dalam simulasi normal.

5. Kesimpulan

Dari hasil simulasi dan analisis, dapat disimpulkan bahwa metode Brute Force cocok digunakan untuk simulasi sederhana dengan jumlah objek yang sedikit karena mudah diimplementasikan. Namun, untuk simulasi dengan jumlah objek yang banyak dan membutuhkan performa tinggi, metode Quadtree jauh lebih efektif dan efisien. Penggunaan Quadtree secara signifikan mengurangi jumlah pengecekan tabrakan sehingga meningkatkan performa simulasi. Oleh karena itu, dalam pengembangan game atau simulasi fisika skala besar, metode Quadtree sangat direkomendasikan dibandingkan Brute Force.
