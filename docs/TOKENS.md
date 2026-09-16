#Zoom Lexical Rules

| Token     | Can start with                    | Can continue with         | Has value? |
|-----------|-----------------------------------|---------------------------|------------|
| `_ident`  | letter (`a`–`z`, `A`–`Z`) or `_`  | letter, digit, or `_`     | yes — name |
| `_int`    | digit (`0`–`9`)                   | digit                     | yes — text |
| `_assign` | `=`                               | n/a — single character    | no         |
| `_plus`   | `+`                               | n/a — single character    | no         |
| `_semi`   | `;`                               | n/a — single character    | no         |

## Keywords

Reserved words are scanned using the same character rules as
`_ident` (see above) — the lexer doesn't distinguish a keyword from
an identifier while scanning. Only after the full word has been
scanned is it checked against a keyword table; if it matches, the
keyword's token is emitted instead of `_ident`. This means a
reserved word can never be used as a variable name.

In practice, all current and planned keywords consist only of
letters, none use `_` or digits, though the lexer would still
correctly tokenize an underscore or digit-containing sequence as
an ordinary identifier if it doesn't match a table entry exactly.

| Keyword | Token   |
|---------|---------|
| `exit`  | `_exit` |

## Identifiers

- Case-sensitive (`exit`, `Exit`, and `EXIT` are three distinct
  pieces of text — only `exit` matches the keyword table).

## Whitespace

- Spaces, tabs, and newlines are skipped and never produce a token.

## Errors

- Any character not covered by the rules above is a lexing error;
  the lexer reports it and exits immediately (no error recovery).