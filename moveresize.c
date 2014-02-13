/*
void
moveresize(const Arg *arg) { */
	/* only floating windows can be moved */
	/*Client *c;
	c = selmon->sel;
	int x, y, w, h, nx, ny, nw, nh, ox, oy, ow, oh;
	char xAbs, yAbs, wAbs, hAbs;
	int msx, msy, dx, dy, nmx, nmy;
	unsigned int dui;
	Window dummy;

	if (!c || !arg)
		return;
	if (selmon->lt[selmon->sellt]->arrange && !c->isfloating)
		return;
	if(sscanf((char *)arg->v, "%d%c %d%c %d%c %d%c", &x, &xAbs, &y, &yAbs, &w, &wAbs, &h, &hAbs) != 8)
		return; */
	/* compute new window position; prevent window from be positioned outside the current monitor */
	/*nw = c->w + w;
	if(wAbs == 'W')
		nw = w < selmon->mw - 2 * c->bw ? w : selmon->mw - 2 * c->bw;

	nh = c->h + h;
	if(hAbs == 'H')
		nh = h < selmon->mh - 2 * c->bw ? h : selmon->mh - 2 * c->bw;

	nx = c->x + x;
	if(xAbs == 'X') {
		if(x < selmon->mx)
			nx = selmon->mx;
		else if(x > selmon->mx + selmon->mw)
			nx = selmon->mx + selmon->mw - nw - 2 * c->bw;
		else
			nx = x;
	}

	ny = c->y + y;
	if(yAbs == 'Y') {
		if(y < selmon->my)
			ny = selmon->my;
		else if(y > selmon->my + selmon->mh)
			ny = selmon->my + selmon->mh - nh - 2 * c->bw;
		else
			ny = y;
	}

	ox = c->x;
	oy = c->y;
	ow = c->w;
	oh = c->h;

	XRaiseWindow(dpy, c->win);
	Bool xqp = XQueryPointer(dpy, root, &dummy, &dummy, &msx, &msy, &dx, &dy, &dui);
	resize(c, nx, ny, nw, nh, True);
*/
	/* move cursor along with the window to avoid problems caused by the sloppy focus */
/*	if (xqp && ox <= msx && (ox + ow) >= msx && oy <= msy && (oy + oh) >= msy)
	{
		nmx = c->x - ox + c->w - ow;
		nmy = c->y - oy + c->h - oh;
		XWarpPointer(dpy, None, None, 0, 0, 0, 0, nmx, nmy);
	}
}
*/

static void
moveresize(const Arg *arg) {
    XEvent ev;
    Monitor *m = selmon;
    Client *c;

    if(!(m->sel && arg && arg->v))
        return;
    if(m->lt[m->sellt]->arrange && !m->sel->isfloating)
        togglefloating(NULL);

    resize(m->sel, m->sel->x + ((int *)arg->v)[0],
        m->sel->y + ((int *)arg->v)[1],
        m->sel->w + ((int *)arg->v)[2],
        m->sel->h + ((int *)arg->v)[3],
        True);

    /* NEW */
    c = m->sel;

    if((m = recttomon(c->x, c->y, c->w, c->h)) != selmon) {
        sendmon(c, m);
        selmon = m;
        focus(NULL);
    }
    /* END OF NEW */

    while(XCheckMaskEvent(dpy, EnterWindowMask, &ev));
}
