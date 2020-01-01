# Binary Database

This is a complete database library made for (Principal of Programming course in University of Shiraz)

## How to use it
* first create a `Context` object
* open/create database
* execute commands with `ctx.exec` to control your data
* read result of each command at `context.res()` and access to result data on `context.rows()`
* at last its better to `close` the context, also `clean` context after you're done with its result

## Commands
Note that all commands are **case-insensitive** you can use how case you want.
1.  `Select(<TableName>, <cols>, <query>)`  OR `Select(<TableName>, <query>, <cols>)` 
    * select some `DataRow`s from table and read their requested columns that matched in requested Query
    * you can 
    * `<TableName>` name of target table bare or wrapped in `"` or `'` or ` (**str-format**)
    * `<cols>` list of column names styled like above and wrapped in `[ ... ]` to load these columns on each result rows, if not provided: all columns of table will be loaded, if you just want `Row` without their data, just pass an empty list (`[]`)
    * `<query>` if provided, limits the resulting rows to match the required *Query*
2.  Comparators Commands: `<cmp>(col, data)`
    * use bellow functions in `<cmp>` to preform compare Queries
    * `EQ` as equals, `NEQ` and `NE` as not-equals
    * `GT` as greater-than, `GE` as greater-than-or-equals
    * `LT` as less-than, `LE` as less-than-or-equals
    * `NGT` is `LE` , `NGE` is `LT` , `NLT` is `GE` , `NLE` is `GT`
    * `In( <col>, <list> )` check value in `<list>` (**lst-format** required), `NIN` is its opposite
3.  Logical Commands:    
    * `NOT( <query> )` inverse the result returned by query inside
    * `AND( <q1> , <q2> , ... )` join all query results and return results presented on all queries
    * `OR( <q1> , <q2> , ... )` merge all query results and return results presented at least on one query
4.  Table Commands:
    * `CreateTable( <name>, {<col:type>, ...} )`
      * create a table named `<name>` with columns with requested type. columns must send in **set-format** `{ ... }` each follows `<col>:<typ>` format.
    * `DeleteTable( <name> )` Delete table `<name>` from database
5.  Data Commands:
    - `Insert( <table>, { <set1> }, { <set2> }, ...)`
        - insert data into `<table>` each data represented by its `<set>` argument (**set-format** is required).
        - each `<set>` contains set of `<col>=<val>` to assign data to each data cell
        - `<val>` type must match `<col>` type
    - `Update( <table>, { <set> }, <query>)`
        - update all rows matched in `<query>` with `<set>` parameter
        - every parameter matches exactly like other functions described above
    - `Remove( <table>, <query> )`
        - Remove all rows in `table` matches in `query`

## Data Type
There is 4 DataType implemented in this Database:
1. `Byte` or`Char` and their arrays `Byte[fixed]`, `Char[fixed]`
2. `Int` and `Int[fixed]` hold any Integers
3. `Real` and `Real[fixed]` holds all Double/Float numbers
4. `Text` variable size string without any restrictions
5. In later versions other types than Text, may able to have Variable-Size array

## VarArgs
You can call `exec` to run requested actions, also you can pass list of `varargs` to fill in the gaps.
* VarArgs Gap Style `${<index>:<typ>}`:
  * `<index>` is optional and if its not provided every gap gets its own index start from 1 and continues.
  * `<typ>` follows like any DataType parameter rules mentioned above.

## Formats
* String Format: `"<text>"`, `'<text>'`, or ``` `<text>` ```
* List Format: `[ <element1>, <element2>, ... ]`
* Set Format: `[ <element1>, <element2>, ... ]`
* Function Format: `<func>( <param1>, <param2>, ... )`
* Parenthesis Format: `( ... )`
* VarArgs Format: `${<index>:<typ>}` or `${<typ>}`
* Column Type Format: `<colName>:<type>`
* Type Format: `<typ>` or `<typ>[<elementCount>]`

## Example
you can see [test-context.cpp](./test/test-context.cpp) and [test-bookstore.cpp](./test/test-bookstore.cpp) in [tests folder](./test/) to see how to use this library
also here some command examples:
```
CREATETABLE(students, id:int, name:text, grade:real, year:byte)

InserT(students, {id=1}, {id=2}, {id=3})

uPDATE(students, {id=10}, eq(id, 2))

select(students,[year])

SELECT(students,eq(year,255))

Select(students,and( nEQ(year,94) , neq(id,1374) ))

```

## TODOs

- [ ] add `OpenDatabase`,`CloseDatabase`, `DeleteDatabase`, `ClearDatabase` database commands
- [ ] add `sort` query command
- [ ] add `like`, `contains`, `exists`, `startsWith`, `endsWith`, 'ignorecase' in string query commands
- [ ] add `CompactTable` or something similar to compact database (remove additional space between rows)
- [ ] add `Len(col)`, `Count(<query>)`, `Max(<col>)`, `Min(<col>)`, `Avg(<col>)`, `Round`, `isEmpty(<col>)`
- [ ] add **Index** meta files, *UniqueIndex*, *PrimaryKeyIndex*
- [ ] add `Comment` to commands (assembly, c, ... style)