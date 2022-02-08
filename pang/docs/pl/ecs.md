# Entity Component System

ECS to architektura którą wybrałem do tego projektu. Dlaczego? Nie wiem, wydawała się spoko i znalazłem jedyną i niepowtarzalną bibliotekę, która jest wykorzystywana w Minecrafcie (oczywiście chodzi mi o [entt](https://github.com/skypjack/entt)).

Jeśli nie znasz się na tej architekturze i jak działa, idź doczytaj bo nie będę cię trzymał za rękę cholera jasna, tylko wyjaśniam jak ją zimplementowałem.

## Byty i Komponenty

Napisałem komponenty tak jak lubię - MINIMALNE i bez żadnego BLOATU. Są one prostymi structami z kilkoma polami, czasami też z jednym, albo wcale (nazywam takie markerami). Komponenty nie mają żadnych funkcji, a ich pola to typy prymitywne, stringi lub enumy. Staram się nie wkładać do komponentów niczego dużego, ponieważ będą przesuwane i zmieniane wiele razy, więc są tyci i malutkie uwu.

Staram się także rozdzielać je, nie na tak wiele jak się da, ale na więcej niż niektórzy lubią, tylko po to żeby były małe: np. `Position` i `Hitbox` - mogłem zrobić z nich jeden komponent i może byłoby łatwiej używać ich w systemach, ale co jakby jakiś byt miał `Position`, ale nie miał `Hitbox`? No właśnie.

Musiałem odejść od swojej reguły minimalizmu przy `Sprite` - mogłem to zrobić w ten sposób, że miałbym komponenty `Sprite` i `AnimatedSprite`, ale wtedy musiałbym je usuwać i spowrotem dodawać np. jak gracz zaczyna chodzić, albo ściana się niszczy. Wolę zamieniać jeden bool i ustawiać kilka wartości zamiast tego.

## Systemy

Wszystkie moje systemy dziedziczą z podstawowej klasy `System`, która zawiera jedynie odniesienie do `entt::registry`, funkcję `update()` i rzeczy wykorzystywane do pauzowania i wznawiania. Poza tym drzewem mam jeszcze drugie - `RenderingSystem`y. Te, poza rzeczami z `System`, mają jeszcze odniesienie do `sf::RenderWindow`. W całej grze używam tych 2 typów `System`ów. Rozdzielenie ich pozwala na rozdzielenie aktualizowania i renderowania, dzięki czemu mogę zapauzować grę poprzez zaprzestanie
wywoływania głównej funkcji `update()`, ale zostawienia `render()`. Systemy znajdują się w 2 wektorach, jeden ma `System`y, a drugi `RenderingSystem`y (miałem z nimi mnóstwo frajdy z moim brakiem wiedzy o pointerach btw). Na każdą aktualizację podaję do każdego z systemów deltę czasu, skalę okna i przełącznik debugowania. Wiem że mógłbym po prostu podać odniesienia do nich w konstruktorze z `Game`, ale przez moje zdecydowanie większe doświadczenie z Javą niż C++, zrobiłem to w ten sposób, a
teraz jest trochę za późno na refaktoryzację, w końcu to nie jest jakiś duży problem.

Przez to jak napisane są `System`y mogę je z łatwością refaktoryzować, np. jeśli zauważę, że jeden z `System`ów zaczyna się robić zbyt duży albo robi 2 kompletnie różne rzeczy zamiast jednej, biorę część jego pracy i tworzę nowy `System`, żeby się nią zajął. W sposobie w jaki napisałem `System`y, możesz znowu zauważyć moją minimalistyczną i BEZBLOATOWĄ filozofię.

Widziałem również że niektórzy implementują systemy poprzez samodzielne funkcje w oddzielnych nagłówkach, i wywołując je gdy nadchodzi potrzeba. Biorąc znowu pod uwagę moje doświadczenie z Javą, nawet nie pomyślałem o tym sposobie, zanim go nie zobaczyłem. Wywoływanie każdej pojedynczej funkcji zamiast iteracji przez wektor i wywoływania jednej funkcji na każdym systemie byłoby pewnie monotonne, ale co ja tam wiem.
