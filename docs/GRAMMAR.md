# Zoom Parsing Grammar

```
factor = ident | int
term   = factor | factor * term | factor / term | factor % term
expr   = term | term + expr | term - expr
instr  = <exit> expr ; | ident = expr ;
```
