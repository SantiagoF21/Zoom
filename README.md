# Zoom

Zoom is a small compiler for a toy language, targeting AArch64 assembly
(Apple Silicon / macOS). It's a learning project: a hand-written lexer,
recursive-descent parser, and AST-walking code generator, built in C++20.

## Pipeline

source (.zm) -> Lexer -> tokens -> Parser -> AST -> Generator -> assembly (.s/.asm)

- **Lexer** (`lexer.hpp`/`.cpp`) — turns source text into a flat token stream.
- **Parser** (`parser.hpp`/`.cpp`) — recursive descent parser, builds an AST
  (`Node::Instruction`) from the token stream.
- **Generator** (`generator.hpp`/`.cpp`) — walks the AST and emits AArch64
  assembly as a string.

## Grammar

See `docs/GRAMMAR.md` for the full grammar and `docs/TOKENS.md` for the
lexical rules (what each token can start/continue with, keyword handling,
etc.).

## Building

```zsh
cmake -S . -B build
cmake --build build
```
The `zoom` executable is placed in `build/`.

## Usage

```zsh
./build/zoom <input.zm> [--debug]
```

Compiles `<input.zm>` and writes the resulting assembly to `program.asm`
in the current directory. `--debug` additionally prints the token stream
and parsed AST.

To assemble and run the output on Apple Silicon:

```zsh
g++ program.asm -o program
./program
echo $?
```

## Tests

Sample `.zm` programs covering the current grammar (including a couple of
deliberately invalid ones for exercising error paths) live in `tests/`.

## License

MIT — see [LICENSE](LICENSE).
