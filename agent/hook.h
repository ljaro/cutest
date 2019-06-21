#ifndef HOOK_H
#define HOOK_H


class Hook
{
public:
    static void install();
private:
    Hook();
    static bool installed;
};

#endif // HOOK_H
