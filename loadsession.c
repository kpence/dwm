void
loadsession()
{
    char name[MAX_TAGLEN];
    FILE* f;
    char* p;
    int i = 0;

    // Get session information
    // TODO -- set the new window locations
    f = fopen("/tmp/.dwm.session", "r");
    if (f != NULL) {
        // Save in a /tmp file
        while (fgets(name, MAX_TAGLEN, f) != NULL
                && i < LENGTH(tags))
        {
            if((p = strchr(name, '\n')))
            *p = '\0';
            strcpy(tags[i++], name);
        }

        // close the file handle
        fclose(f);

        // delete the file
        remove("/tmp/.dwm.session");
    }

    // TODO not sure if i want to do this but drawbars
    drawbars();
}
