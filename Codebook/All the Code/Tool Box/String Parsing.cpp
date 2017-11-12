// Splits a string 'work by the delimiters in the set 'delim', ignoring multiple occurences of each delimiter.
vector < string > split(string work, set < char > delim) {
    vector < string > flds;
    if (!flds.empty()) flds.clear();
    string buf = "";
    int i = 0;
    while (i < (int) work.size()) {
        bool is_delimiter = delim.count(work[i]);
        if (is_delimiter == false) {
            buf += work[i];
        } else if (((int) buf.size()) > 0) {
            flds.push_back(buf);
            buf = "";
        }
        i++;
    }
    if ((int) buf.size()) {
        flds.push_back(buf);
    }
    return flds;
}

// stod() -> converts a string to a double
// stoi() -> converts a string to a int
// stoll() -> converts a string to a long long

// Sometime, if we use getline() after using cin, we might require a cin.ignore() statement.
// Keep reading lines till EOF, stores string in s, reads SENTENCES, including SPACES.
while (getline(cin, s))

