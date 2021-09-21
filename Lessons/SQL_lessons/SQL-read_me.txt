=====СОЗДАНИЕ/УДАЛЕНИЕ/ИЗМЕНЕНИЕ=====
1) Cоздание / Удаление

	БД: 
		CREATE DATABASE имя; / DROP DATABASE имя;
	
	Таблицы:

		CREATE TABLE ktt.infobjects (
  		id_abs bigserial NOT NULL, -- Уникальный идентификатор
  		infres_id bigint NOT NULL, -- Идентификатор информационного ресурса, которому принадлежит информационный объект
  		infobj_token_value character(32) NOT NULL,
  		infobj_name character varying,
  		infobj_type integer NOT NULL DEFAULT 0, -- Тип (FASTSQL запрос или доступ по API)
  		
  		CONSTRAINT infobjects_pkey PRIMARY KEY (id_abs),
  		CONSTRAINT fkey_infobjects_infres_id FOREIGN KEY (infres_id)
      		REFERENCES ktt.infresources (id_abs) MATCH SIMPLE
      		ON UPDATE NO ACTION ON DELETE CASCADE ON CHMAC  ,
		CONSTRAINT infobjects_infobj_token_value_key UNIQUE (infobj_token_value)
		);

		DROP TABLE table;

2) Изменение таблиц

	ALTER TABLE table ADD row <type>;
	ALTER TABLE table DROP COLUMN row;


=====ИЗВЛЕЧЕНИЕ=====
1) Извлечение данных из таблиц

  SELECT * FROM Table;			-- выбор всех столбцов
  SELECT row1, row2, row3 FROM Table;	# выбор определенных столбцов
  SELECT DISTINCT row1 FROM Table;	/* вывести только уникальные записи для столбца */
  SELECT DISTINCT row1 FROM Table LIMIT 10 OFFSET 1;	-- вывод ограниченного числа записи со смещением (вторая строка, а не первая);

2) Сортировка

	SELECT * FROM Table ORDER BY row1, row2;  
	SELECT * FROM Table ORDER BY 1, 2;
	SELECT * FROM Table ORDER BY row1, row2 ASC(DESC);

3) Фильтрация

	Числа:

		SELECT * FROM Table WHERE row1 = something;
		SELECT * FROM Table WHERE row1 BETWEEN 5 AND 10;
		SELECT * FROM Table WHERE row1 IS NULL;			-- ACHTUNG! SELECT * FROM Table WHERE row = NULL; НЕ РАБОТАЕТ(!)		
	
		Допустимые операторы:

			=
			<>
			!=
			<
			<=
			!<
			>
			>=
			!>
			BETWEEN
			IS NULL

	Строки:

	SELECT * FROM Table WHERE row1 = 'something';
	SELECT * FROM Table WHERE row1 <> 'something';	
	SELECT * FROM Table WHERE row1 LIKE '%someth%';
	SELECT * FROM Table WHERE row1 LIKE 's%g';
	SELECT * FROM Table WHERE row1 LIKE '__mething';		
	SELECT * FROM Table WHERE row1 LIKE '[st]%';		-- найти все строки начинающиеся с символов s и t
	SELECT * FROM Table WHERE row1 LIKE '[^s]%';		-- найти все строки кроме начинающихся с символов s и t
	SELECT * FROM Table WHERE NOT row1 LIKE '[st]%';	-- найти все строки начинающиеся с символов s и t (аналог)

	Допустимые логические операции при расширенной фильтрации:

		AND
		OR
		NOT

		SELECT * FROM Table WHERE row1 = 'something' AND row2 = 'something_else';
		SELECT * FROM Table WHERE row1 = 'something' OR row1 = 'something_else';		

		IN/NOT IN 

		SELECT * FROM Table WHERE row1 IN ('something', 'something_else');	-- в скобках может быть подзапрос SELECT и т.д.
		SELECT * FROM Table WHERE row1 NOT IN ('something', 'something_else')	/* NOT может использоваться с BETWEEN и всегда должен стоять 
																																			
перед тем, что он отрицает */

4) Вычислимые поля

	SELECT row1*constant AS new_title FROM Table WHERE row1 = 5;

		Допустимые операторы:

			+
			-
			*
			/

5) Функции обработки

	Текстовые

		LEFT() / RIGHT()	-- левая / правая часть строки
		LOWER() / UPPER()	-- перевод в нижний (верхний регистр)
		LTRIM() / RTRIM()	-- удаление пробельных символов слева(справа)
		LENGTH() / LEN()	-- длина строки
		SOUNDEX()		-- фонетическое подобие (как звучит строка)

		SELECT UPEER(row1) AS new_title FROM table WHERE row2 = 42;

	Числовые

		ABS()
		COS()
		EXP()
		PI()
		SIN()
		SQRT()
		TAN()

	Итоговые

		AVG()
		COUNT()
		MAX()
		MIN()
		SUM()

		SELECT AVG(row1) AS average FROM table; -- подсчет среднего, строки с NULL игнорируются
		SELECT COUNT(*) FROM table;		-- подсчет общего количества строк, включая NULL
		SELECT COUNT(row1) FROM table;		-- подсчет количества строк для row1 без NULL
		SELECT SUM(row1*constant) AS sum_multiplied FROM table; -- подсчет суммы перемноженной на константу и т.д.

		Можно использовать DISTINCT:

		SELECT SUM(DISTINCT row1) AS sum_unique FROM table;	-- подсчет суммы уникальных элементов
		SELECT COUNT(DISTINCT row1) AS sum_unique FROM table;	-- подсчет количества уникальных элементов

		И комбинировать:

		SELECT SUM(DISTINCT row1) AS sum_unique, MIN(row2) AS min_value, MAX(row3) AS max_value FROM table;

6) Группы

	SELECT row1, COUNT(*) AS quantity FROM table GROUP BY row1; - сгруппировать и отобразить количество записей для каждого уникального значения row1

	SELECT row1, COUNT(*) AS quantity FROM table GROUP BY row1 HAVING COUNT(*) >= 2; - сгруппировать и отобразить количество записей для каждого уникального значения row1, в котором записей >= двух

	ACHTUNG! Порядок следования операторов в подобных запросах:

		SELECT
		FROM
		WHERE
		GROUP BY
		HAVING
		ORDER BY

7) Подзапросы (допускают любые операторы из пунктов выше)

	SELECT row1 FROM table1 WHERE row2 IN (SELECT row2 FROM table2 WHERE row2 = 42);

8) Объединения таблиц простое

	Внутреннее (или объединение "по равенству")

	Вариант 1: SELECT row1, row2, rowN FROM Table1, Table2 WHERE Table1.row1 = Table2.row1;
	Вариант 2: SELECT row1, row2, rowN FROM Table1 INNER JOIN Table2 ON Table1.row1 = Table2.row1;

	Нескольких таблиц по равенству

	SELECT row1, row2, rowN FROM Table1, Table2, Table3 
	WHERE Table1.row1 = Table2.row1 AND Table2.row2 = Table3.row3;

	Или c псевдонимами таблиц:

	SELECT row1, row2, rowN FROM Table1 AS T1, Table2 AS T2, Table3 AS T3 
	WHERE T1.row1 = T2.row1 AND T2.row2 = T3.row3;

9) Объединение таблиц расширенное

	Естественное объединение

	SELECT T1.*, T2.row1. T3.row3
	FROM table1 AS T1, table2 AS T2, table3 AS T3
	WHERE T1.row1 = T2.row1 AND T2.row3 = T3.row3;

	Внешнее (когда объединяются таблицы не имеющие пар)

	SELECT T1.row1, T2.row2 FROM T1 LEFT(RIGHT, FULL) OUTER JOIN T2 ON T1.row1 = T2.row1; 

10) Комбинированные запросы

	SELECT row1 FROM table1 WHERE row1 IN (...)
	UNION (UNION ALL - для всех вхождений без удаления дубликатов)
	SELECT row2 FROM table2 WHERE row2 IN (...);

=====ДОБАВЛЕНИЕ=====

1) Добавление полных строк

	INSERT INTO table1 VALUES (value1, value2, ..., valueN);
	INSERT INTO table1(row1, row2) VALUES (value1, value2);
	INSERT INTO table1(row1, row2) SELECT row1, row2 FROM table1;

2) Копирование из одной таблицы в другую

	SELECT * INTO dest_table FROM source_table;
	CREATE TABLE dest_table AS SELECT * FROM source_table;

3) Обновление и удаление

	UPDATE table SET row2 = 'foo', row3 = 'bar' WHERE row1 = 'bizz'; 	
	DELETE FROM table WHERE row1 = 'foo';

=====ПРЕДСТАВЛЕНИЯ=====

1) Создание / удаление

	CREATE VIEW my_view AS SELECT * FROM table WHERE row = 'foo';
	DROP VIEW my_view;

2) SELECT row1 FROM my_view WHERE row1 = 42;

=====ТРАНЗАКЦИИ=====

	BEGIN;
	COMMIT;
	ROLLBACK;

=====LEVEL-UP=====

	INDEX
	TRIGGER
	Хранимые процедуры
