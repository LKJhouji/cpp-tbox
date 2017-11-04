#include <iostream>
#include <tbox/event/loop.h>
#include <tbox/event/signal_item.h>
#include <tbox/network/stdio_stream.h>

using namespace std;
using namespace tbox;

//! 一个终端的回显示例
int main()
{
    auto sp_loop = event::Loop::New();
    auto sp_stdin = new network::StdinStream(sp_loop);
    auto sp_stdout = new network::StdoutStream(sp_loop);

    sp_stdin->bind(sp_stdout);

    sp_stdin->enable();
    sp_stdout->enable();

    auto sp_exit = sp_loop->newSignalItem();
    sp_exit->initialize(SIGINT, event::Item::Mode::kOneshot);
    sp_exit->setCallback(
        [=] {
            cout << "Info: Exit Loop" << endl;
            sp_loop->exitLoop();
        }
    );
    sp_exit->enable();

    cout << "Info: Start Loop" << endl;
    sp_loop->runLoop();
    cout << "Info: End Loop" << endl;

    delete sp_exit;
    delete sp_stdout;
    delete sp_stdin;
    delete sp_loop;
    return 0;
}
