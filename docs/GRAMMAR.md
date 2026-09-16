#Zoom Parsing Grammar

    term = ident | int
    expr = term | term + expr
    instr = <exit> expr; | ident = expr;
