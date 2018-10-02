# Делительная головка на Arduino Mega и 480x320 TFT

* Все настройки прошивки - в файле <b>CONFIG.h</b>;
* Схема соединений - в папке <b>Schematic</b> (проект для программы sPlan);
* Arduino Mega 2560;
* Дисплей - вот такой, на 3.2'', 480х320 на контроллере ILI9481: https://ru.aliexpress.com/item/3-2-3-5-480-320-TFT/32917044135.html;
* Клавиатура - MPR121, ёмкостная, на шину I2C (использовать не обязательно): https://ru.aliexpress.com/item/MPR121-Capacitive-Touch-Keypad-Shield-module-sensitive-key-keyboard/32642505921.html;
* Энкодер - вот такой (да любой можно использовать, главное, чтобы подтяжка его выводов была к питанию): https://ru.aliexpress.com/item/Free-Shipping-Rotary-Encoder-Module-Brick-Sensor-Development-for-arduino-Dropshipping-KY-040/32262356075.html;
* Кнопки - любые по вкусу;
* Шаговый и его драйвер - любой по вкусу;
* Индикация работы шагового - светодиод и резистор, использовать индикацию - опционально.

## Принцип управления

* Перемещения по экранным кнопкам - происходят при помощи вращения энкодера;
* Клик по экранной кнопке - происходит при помощи нажатия кнопки энкодера;
* Уменьшение/увеличение значения параметра на экране - при помощи вращения энкодера, или набора на клавиатуре;
* Если на экране только одно редактируемое значение - то его изменение осуществляется при помощи вращения энкодера, или вводом с клавиатуры. Кнопка "< НАЗАД" на таком экране - всегда подсвечена, и переход на предыдущий экран осуществляется кликом кнопки энкодера;
* Если на экране есть несколько редактируемых полей, то активное редактируемое поле - подсвечивается, и ввод доступен именно в него. Для смены фокуса ввода - клик кнопкой энкодера. В описываемом случае кнопка "< НАЗАД" на экране - также служит полем ввода, и клик на ней осуществляется не по клику на энкодере - а вращением энкодера в любую сторону.

## Схема соединений

<img src="Schematic/Schematic.png" style="width:100%"/>
