#Python code to generate NBT functions

prntDecl = False;
prntImpl = True;

types = ["bool", "char", "uchar", "short", "ushort", "int", "uint", "long", "ulong", "llong", "ullong", "float", "double", "ldouble", "string", "NBT_Node"];

#for declarations
if(prntDecl):
    for t in types:
        print("void set" + t[0].upper() + t[1:] + "(string name, " + t + " " + t[0] + ");");
        print(t + " get" + t[0].upper() + t[1:] + "(string name);");
        print();

#for implementations
if(prntImpl):
    print("""set<string>& NBT_Node::getKeys(){
    if(dirtyKeys){
        masterKeys.clear();
        """);

    for t in types:
        print("\tmap<string, " + t + ">::iterator " + t + "Iter;");
    
    print();

    for t in types:
        print("\tfor (" + t + "Iter = " + t + "s.begin(); " + t + "Iter != " + t + "s.end(); " + t + "Iter++){");
        print("\t\tmasterKeys.insert(" + t + "Iter->first);\n\t}\n");

    print("\tdirtyKeys = false;\n}\n\nreturn masterKeys;\n}\n");

    for t in types:
        print("void NBT_Node::set" + t[0].upper() + t[1:] + "(string name, " + t + " " + t[0] + "){\n\t" + t + "s[name] = " + t[0] + ";\n\n\tdirtyKeys = true;\n}");
        print();
        print(t + " NBT_Node::get" + t[0].upper() + t[1:] + "(string name){\n\treturn " + t + "s[name];\n}");
        print();
