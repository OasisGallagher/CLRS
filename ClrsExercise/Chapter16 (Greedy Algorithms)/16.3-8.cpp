/*
 8bits的字符, 共有256种, 文件中有n个这样的字符, 那么, 每个字符都有256
 种选择, 所以, 可能的不同文件数为256^n.
 因为编码的过程, 必须对每个不同的文件, 生成一种不同的编码, 所以, 为了
 编码这n个字符, 编码文件也需要256^n种.
 所以, 为了表示着256^n种不同的编码文件, 就至少需要log(256^n) = 8n个bits.
 所以, 编码文件至少需要n个字符, 与文件本身所用的字符数相同, 没有进行
 压缩.
 */