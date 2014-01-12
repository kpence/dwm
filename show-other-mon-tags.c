if (mons->next) {
    Monitor *om = (m == mons) ? m->next : mons;
    col = (om->tagset[om->seltags] & 1 << i) ? dc.spec : col;
}
