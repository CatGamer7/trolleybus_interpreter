# Intro
C++ Interpreter for a simple scripting language following the book "Crafting Interpreters" by Robert Nystrom.

# Full syntax rules
TODO (:sad:)

Quick tips:
1. Lines are terminated by semicolon;
2. For, While, If, else if, else follow C syntax;
3. Assignment is an expression. Find out what this entails;
4. Printing to stdout is a statement that follows "print {expression}" rule.


# Examples
program 1:
```
if ("cat") {
    print "meow";
}
else {
    print "anti-meow";
}
```
output 1:
```
> ./tpp.exe hello.tpp
> "meow"
```
program 2:
```
if (0) {
    print "meow";
}
else {
    print "anti-meow";
}
```
output 2:
```
> ./tpp.exe hello.tpp
> "anti-meow"
```
